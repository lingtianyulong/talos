/**
 * File header
 * @author: talos
 * @date: 2026-03-01
 * @description: 文件头
 * @version: 1.0.0
 */
use bytemuck::{Pod, Zeroable};

#[repr(C)]
#[derive(Debug, Clone, Copy, bytemuck::Pod, bytemuck::Zeroable)]
pub struct FileHeader {
    pub magic: u32,
    pub version: u32,
    pub reserved: u32,
}


impl FileHeader {
    pub fn new() -> Self {
        Self {
            magic: 0xDEADBEEF,
            version: 1,
            reserved: 0,
        }
    }
}


#[repr(C)]
#[derive(Debug, Clone, Copy, Pod, Zeroable)]
pub struct ChunkHeader {
    pub timestamp: u64,
    pub data_len: u32,
    pub crc32: u32
}