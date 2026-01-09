/// @author: talos
/// @date: 2025-12-02
/// @description: 加密接口
/// @version: 1.0.0
///
/// @param: data: &str
/// @return: String
///
/// @example:
/// let data = "hello";
/// let encrypted = encrypt.encrypt(data);
/// println!("encrypted: {}", encrypted);
///
/// @note: 需要实现加密和解密接口
#[allow(dead_code)]
pub trait Encrypt {
    fn encrypt(&self, data: &str) -> Result<String, Box<dyn std::error::Error>>;
    fn decrypt(&self, data: &str) -> Result<String, Box<dyn std::error::Error>>;
    fn verify(&self, password: &str, store_password: &str) -> Result<bool, Box<dyn std::error::Error>>;
}