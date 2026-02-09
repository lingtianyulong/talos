//
// Created by lingt on 2025/12/3.
//

#include "encrypt_util.h"

namespace talos::encrypt {
const char* EncryptUtil::encrypt(gsl::not_null<const char*> password) {
    return encrypt_password_argon2(password);
}

bool EncryptUtil::verify(gsl::not_null<const char*> password,
                         gsl::not_null<const char*> storedPwd) {
    return verify_password_argon2(password, storedPwd);
}

void EncryptUtil::release(gsl::not_null<char*> password) {
    release_encrypted_password(password);
}
}  // namespace talos::encrypt
