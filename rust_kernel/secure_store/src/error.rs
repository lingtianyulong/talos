use thiserror::Error;

#[derive(Debug, Error)]
pub enum SecureStoreError {
    #[error("item not found")]
    NotFound,

    #[error("permission denied")]
    PermissionDenied,

    #[error("backend error: {0}")]
    Backend(String),
}
