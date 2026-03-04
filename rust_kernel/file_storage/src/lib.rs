mod local_files;


pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    use std::sync::Arc;
    use std::thread;
    use std::time::{SystemTime, UNIX_EPOCH};
    use local_files::local_storage::{FileReader, FileWriter};

    use rkyv::{Archive, Deserialize, Serialize};
    use bytecheck::CheckBytes;

    /// 测试数据结构
    #[derive(Archive, Deserialize, Serialize, Debug, CheckBytes, Clone)]
    #[bytecheck(verify)]
    pub struct Record {
        pub id: u64,
        pub value: f64,
    }


    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }

    #[test]
    fn test_local_storage_write_and_read_all() -> anyhow::Result<()> {
        let mut path = std::env::temp_dir();
        let unique = SystemTime::now()
            .duration_since(UNIX_EPOCH)?
            .as_nanos();
        path.push(format!("file_storage_test_{unique}.bin"));
        let path_str = path.to_string_lossy().to_string();

        let chunk = vec![
            Record { id: 1, value: 1.5 },
            Record { id: 2, value: 3.5 },
        ];

        let writer = Arc::new(FileWriter::new(&path_str)?);
        writer.append(&chunk)?;
        drop(writer);

        let reader = FileReader::open(&path_str)?;
        let all = reader.read_all::<Record>()?;

        assert_eq!(all.len(), 1);
        assert_eq!(all[0].len(), 2);
        assert_eq!(all[0][0].id, 1);
        assert_eq!(all[0][0].value, 1.5);
        assert_eq!(all[0][1].id, 2);
        assert_eq!(all[0][1].value, 3.5);

        fs::remove_file(&path)?;
        Ok(())
    }

    #[test]
    fn test_local_storage_concurrent_write_and_read() -> anyhow::Result<()> {
        let mut path = std::env::temp_dir();
        let unique = SystemTime::now()
            .duration_since(UNIX_EPOCH)?
            .as_nanos();
        path.push(format!("file_storage_concurrent_test_{unique}.bin"));
        let path_str = path.to_string_lossy().to_string();

        let writer = Arc::new(FileWriter::new(&path_str)?);
        let writer_threads = 8usize;
        let records_per_chunk = 16usize;

        let mut write_handles = Vec::with_capacity(writer_threads);
        for t in 0..writer_threads {
            let writer = Arc::clone(&writer);
            write_handles.push(thread::spawn(move || -> anyhow::Result<()> {
                let chunk: Vec<Record> = (0..records_per_chunk)
                    .map(|i| Record {
                        id: (t * 10_000 + i) as u64,
                        value: (t as f64) + (i as f64) / 100.0,
                    })
                    .collect();
                writer.append(&chunk)?;
                Ok(())
            }));
        }

        for handle in write_handles {
            handle
                .join()
                .map_err(|_| anyhow::anyhow!("writer thread panicked"))??;
        }
        drop(writer);

        let reader = Arc::new(FileReader::open(&path_str)?);
        let reader_threads = 4usize;
        let expected_chunk_count = writer_threads;
        let expected_record_count = writer_threads * records_per_chunk;

        let mut read_handles = Vec::with_capacity(reader_threads);
        for _ in 0..reader_threads {
            let reader = Arc::clone(&reader);
            read_handles.push(thread::spawn(move || -> anyhow::Result<()> {
                let all = reader.read_all::<Record>()?;
                let total_records: usize = all.iter().map(|c| c.len()).sum();
                assert_eq!(all.len(), expected_chunk_count);
                assert_eq!(total_records, expected_record_count);
                Ok(())
            }));
        }

        for handle in read_handles {
            handle
                .join()
                .map_err(|_| anyhow::anyhow!("reader thread panicked"))??;
        }

        fs::remove_file(&path)?;
        Ok(())
    }
}
