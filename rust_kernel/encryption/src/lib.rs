mod api;
mod argon2_encrypt;
mod encrypt_factory;
mod encrypt_trait;

pub use api::*;

#[cfg(test)]
mod tests {
    use super::*;
    use encrypt_factory::{EncryptType, create_encrypt};

    #[test]
    fn test_argon2_encrypt() {
        let argon2_encrypt = create_encrypt(EncryptType::Argon2);
        let password = "123456";

        // 1. 验证加密成功 - 不应返回错误
        let encrypted = argon2_encrypt.encrypt(password).unwrap();
        println!("encrypted: {}", encrypted);

        // 2. 验证加密结果不为空
        assert!(!encrypted.is_empty(), "加密结果不应为空");

        // 3. 验证加密结果格式正确 - Argon2 哈希应该以 $argon2 开头
        assert!(
            encrypted.starts_with("$argon2"),
            "加密结果格式不正确，应该以 $argon2 开头"
        );

        // 4. 验证可以用原密码验证通过 - 这是验证加密成功的关键
        assert!(
            argon2_encrypt.verify(password, &encrypted).unwrap(),
            "加密后应该能用原密码验证通过"
        );

        // 5. 验证相同密码每次加密结果不同（因为有随机 salt）
        let encrypted2 = argon2_encrypt.encrypt(password).unwrap();
        assert_ne!(
            encrypted, encrypted2,
            "相同密码每次加密结果应该不同(因为有随机 salt)"
        );

        // 6. 验证两次加密的结果都能用原密码验证通过
        assert!(
            argon2_encrypt.verify(password, &encrypted2).unwrap(),
            "第二次加密的结果也应该能用原密码验证通过"
        );
    }

    #[test]
    fn test_argon2_verify() {
        let argon2_encrypt = create_encrypt(EncryptType::Argon2);
        let password = "123456";
        let encrypted = argon2_encrypt.encrypt(password).unwrap();
        assert_eq!(argon2_encrypt.verify(password, &encrypted).unwrap(), true);

        // 测试验证失败的情况
        let wrong_password = "wrong_password";
        assert_eq!(
            argon2_encrypt.verify(wrong_password, &encrypted).unwrap(),
            false
        );
    }
}
