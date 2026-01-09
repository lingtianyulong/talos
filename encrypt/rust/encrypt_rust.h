//
// Created by lingt on 2025/12/3.
//

#ifndef TALOS_ENCRYPT_RUST_H
#define TALOS_ENCRYPT_RUST_H

#ifdef __cplusplus
extern "C" {
#endif

// 将密码进行加密
const char *encrypt_password_argon2(const char *password);

// 校验密码
bool verify_password_argon2(const char *password, const char *store_password);

// 释放由 encrypt_password_argon2 生成的密码
void release_encrypted_password(char *password);

#ifdef __cplusplus
}
#endif

#endif //TALOS_ENCRYPT_RUST_H
