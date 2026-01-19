//
// Created by lingt on 2025/12/3.
//

#include "encrypt_util.h"

namespace talos::encrypt
{
    const char *EncryptUtil::encrypt(const char *password)
    {
        return encrypt_password_argon2(password);
    }

    bool EncryptUtil::verify(const char *password, const char *storedPwd)
    {
        return verify_password_argon2(password, storedPwd);
    }

    void EncryptUtil::release(char *password)
    {
        release_encrypted_password(password);
    }
}
