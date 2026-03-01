use crate::local_files::common::{FileHeader, ChunkHeader};
use bytecheck::CheckBytes;
use rkyv::{Archive, Deserialize, Serialize};
use std::fs::{ File, OpenOptions };
use std::io::{ Seek, SeekFrom, Write };
use std::time::{ SystemTime, UNIX_EPOCH };
use anyhow::Result;
use crc32fast::Hasher;
use rkyv::access;
use rkyv::api::high::HighSerializer;
use rkyv::rancor::Error as RkyvError;
use rkyv::ser::allocator::ArenaHandle;
use rkyv::util::AlignedVec;
use memmap2::Mmap;


#[derive(Archive, Deserialize, Serialize, Debug, CheckBytes, Clone)]
#[bytecheck(verify)]
pub struct Record {
    pub id: u64,
    pub value: f64,
}

pub struct FileWriter {
    file: File,
}

#[allow(dead_code)]
impl FileWriter {
    fn alignment_padding(offset: u64, align: u64) -> usize {
        let rem = offset % align;
        if rem == 0 {
            0
        } else {
            (align - rem) as usize
        }
    }

    pub fn new(path: &str) -> Result<Self> {
        let mut file = OpenOptions::new()
            .create(true)
            .write(true)
            .truncate(true)
            .open(path)?;

        // 写文件头
        if file.metadata()?.len() == 0 {
            let header = FileHeader::new();
            file.write_all(bytemuck::bytes_of(&header))?;
        }

        file.seek(SeekFrom::End(0))?;
        Ok(Self { file })
    }
  
   fn current_ts() -> u64 {
        SystemTime::now().duration_since(UNIX_EPOCH).unwrap().as_millis() as u64
    }

    pub fn append<T> (&mut self, data: &[T]) -> Result<()>
    where 
        T: Archive + Clone + for<'a> Serialize<HighSerializer<AlignedVec, ArenaHandle<'a>, RkyvError>>, {
        // 序列化
        let bytes = rkyv::to_bytes::<RkyvError>(&data.to_vec())?;

        // CRC
        let mut hasher = Hasher::new();
        hasher.update(&bytes);
        let crc = hasher.finalize();

        let header = ChunkHeader {
            timestamp: Self::current_ts(),
            data_len: bytes.len() as u32,
            crc32: crc,
        };

        self.file.write_all(bytemuck::bytes_of(&header))?;
        let pad = Self::alignment_padding(self.file.stream_position()?, 8);
        if pad > 0 {
            self.file.write_all(&vec![0u8; pad])?;
        }
        self.file.write_all(&bytes)?;
        Ok(())
    } 
}


#[allow(dead_code)]
pub struct FileReader {
    mmap: Mmap,
}

#[allow(dead_code)]
impl FileReader {
    fn alignment_padding(offset: usize, align: usize) -> usize {
        if align == 0 {
            return 0;
        }
        (align - (offset % align)) % align
    }

    pub fn open(path: &str) -> Result<Self> {
        let file = File::open(path)?;
        let mmap = unsafe { Mmap::map(&file)? };
        // 校验文件头
        let header = bytemuck::pod_read_unaligned::<FileHeader>(
            &mmap[..std::mem::size_of::<FileHeader>()],
        );
        if header.magic != 0xDEADBEEF {
            anyhow::bail!("Invalid file header");
        }
        Ok(Self { mmap })
    }
    
    pub fn iterate<T>(&self) -> Result<()>
    where
        T: Archive,
        T::Archived: std::fmt::Debug
            + for<'a> CheckBytes<rkyv::api::high::HighValidator<'a, RkyvError>>,
    {
        let mut offset = std::mem::size_of::<FileHeader>();

        while offset < self.mmap.len() {
            let header = bytemuck::pod_read_unaligned::<ChunkHeader>(
                &self.mmap[offset..offset + std::mem::size_of::<ChunkHeader>()],
            );
            offset += std::mem::size_of::<ChunkHeader>();
            offset += Self::alignment_padding(offset, 8);

            let data_slice = &self.mmap[offset..offset + header.data_len as usize];

            // CRC 校验
            let mut hasher = Hasher::new();
            hasher.update(data_slice);
            if hasher.finalize() != header.crc32 {
                anyhow::bail!("CRC mismatch");
            }

            // 安全访问
            let archived =
                access::<rkyv::vec::ArchivedVec<T::Archived>, RkyvError>(data_slice)?;
            for r in archived.iter() {
                println!("{:?}", r);
            }

            offset += header.data_len as usize;
        }

        Ok(())
    }

    /// 读取所有 chunk 并返回到内存
    pub fn read_all<T>(&self) -> anyhow::Result<Vec<Vec<T>>>
    where
        T: Archive + Clone,     // 需要 Clone 才能从 Archived 转回普通结构体
        T::Archived: for<'a> CheckBytes<rkyv::api::high::HighValidator<'a, RkyvError>>
            + Deserialize<T, rkyv::api::high::HighDeserializer<RkyvError>>,
    {
        let mut offset = std::mem::size_of::<FileHeader>();
        let mut all_chunks: Vec<Vec<T>> = Vec::new();

        while offset < self.mmap.len() {
            let header: ChunkHeader = bytemuck::pod_read_unaligned(
                &self.mmap[offset..offset + std::mem::size_of::<ChunkHeader>()],
            );
            offset += std::mem::size_of::<ChunkHeader>();
            offset += Self::alignment_padding(offset, 8);

            let data_slice = &self.mmap[offset..offset + header.data_len as usize];

            // 校验 CRC
            let mut hasher = Hasher::new();
            hasher.update(data_slice);
            if hasher.finalize() != header.crc32 {
                anyhow::bail!("CRC mismatch");
            }

            // 安全访问
            let archived =
                access::<rkyv::vec::ArchivedVec<T::Archived>, RkyvError>(data_slice)?;

            // 转回普通结构体 Vec<T>
            let mut chunk_vec: Vec<T> = Vec::with_capacity(archived.len());
            for r in archived.iter() {
                chunk_vec.push(rkyv::deserialize::<T, RkyvError>(r)?);
            }

            all_chunks.push(chunk_vec);
            offset += header.data_len as usize;
        }

        Ok(all_chunks)
    }
}

