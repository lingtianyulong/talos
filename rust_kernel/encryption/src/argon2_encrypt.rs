/// @author: talos
/// @date: 2025-12-02
/// @description: Argon2 加密实现
/// @version: 1.0.0
///
/// @param: data: &str
/// @return: String
///
/// @example:
/// let data = "hello";
/// let encrypted = argon2_encrypt(data);
/// println!("encrypted: {}", encrypted);
///
/// @note: 需要使用 Argon2 库进行加密

use crate::encrypt_trait::Encrypt;
use argon2::{ 
    password_hash::{
        PasswordHash,
        PasswordHasher,
        PasswordVerifier,
        SaltString,
    },
    Argon2,
 };

 use rand::rngs::OsRng;
 use std::error::Error;


pub struct Argon2Encrypt;

impl Default for Argon2Encrypt {
    fn default() -> Self {
        Self {}
    }
}

impl Encrypt for Argon2Encrypt {
    fn encrypt(&self, data: &str) -> Result<String, Box<dyn Error>> {
        let argon2 = Argon2::default();
        let mut rng = OsRng;
        let salt = SaltString::generate(&mut rng);
        let password_hash = match argon2.hash_password(data.as_bytes(), &salt) {
            Ok(hash) => hash,
            Err(e) => return Err(format!("Argon2 encrypt error: {}", e).into())
        };
        Ok(password_hash.to_string())
    }

    fn decrypt(&self, _data: &str) -> Result<String, Box<dyn Error>> {
        Ok("".to_string()) // TODO: 实现解密
    }
    
    fn verify(&self, password:&str, store_password:&str) -> Result<bool, Box<dyn Error>> {
        let parsed_hash = match PasswordHash::new(store_password) {
            Ok(hash) => hash,
            Err(e) => return Err(format!("Invalid hash: {}", e).into())
        };
        let argon2 = Argon2::default();
        let result = argon2.verify_password(password.as_bytes(), &parsed_hash);

        match result {
            Ok(_) => Ok(true),
            Err(_) => Ok(false),
        }
    }
    
}

#[allow(dead_code)]
impl Argon2Encrypt {
    pub fn new() -> Self {
        Self {}
    }
}