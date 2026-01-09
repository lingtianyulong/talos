use crate::encrypt_trait::Encrypt;
use crate::argon2_encrypt::Argon2Encrypt;

#[allow(dead_code)]
pub enum EncryptType {
    Argon2,
}

#[allow(dead_code)]
pub fn create_encrypt(encrypt_type: EncryptType) -> Box<dyn Encrypt> {
    match encrypt_type {
        EncryptType::Argon2 => Box::new(Argon2Encrypt::new()),
    }
}


