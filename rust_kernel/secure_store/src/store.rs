use crate::error::SecureStoreError;
use keyring::Entry;

pub struct SecureStore {
    service: String,
}

impl SecureStore {
    pub fn new(service: impl Into<String>) -> Self {
        Self {
            service: service.into(),
        }
    }

    fn entry(&self, key: &str) -> Result<Entry, SecureStoreError> {
        Entry::new(&self.service, key).map_err(map_error)
    }

    pub fn set(&self, key: &str, value: &str) -> Result<(), SecureStoreError> {
        let entry = self.entry(key)?;
        entry.set_password(value).map_err(map_error)?;
        Ok(())
    }

    pub fn get(&self, key: &str) -> Result<String, SecureStoreError> {
        let entry = self.entry(key)?;
        let password = entry.get_password().map_err(map_error)?;
        Ok(password)
    }

    pub fn delete(&self, key: &str) -> Result<(), SecureStoreError> {
        let entry = self.entry(key)?;
        entry.delete_credential().map_err(map_error)?;
        Ok(())
    }
}

#[allow(unreachable_patterns, non_snake_case, unused_variables)]
fn map_error(err: keyring::Error) -> SecureStoreError {
    use keyring::Error::*;
    match err {
        NoEntry => SecureStoreError::NotFound,
        PermissionDenied => SecureStoreError::PermissionDenied,
        other => SecureStoreError::Backend(other.to_string()),
    }
}
