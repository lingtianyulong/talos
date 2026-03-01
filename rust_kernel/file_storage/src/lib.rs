mod local_files;


pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    use std::time::{SystemTime, UNIX_EPOCH};
    use local_files::local_storage::{FileReader, FileWriter, Record};

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

        let mut writer = FileWriter::new(&path_str)?;
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
}
