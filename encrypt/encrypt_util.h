//
// Created by lingt on 2025/12/3.
//

#ifndef TALOS_ENCRYPT_UTIL_H
#define TALOS_ENCRYPT_UTIL_H

#include "rust/encrypt_rust.h"

namespace talos::encrypt {
    class EncryptUtil {
        EncryptUtil() = default;
    public:
        ~EncryptUtil() = default;

        static const char* encrypt(const char* password);
        static bool verify(const char* password, const char* stordPwd);
        static void release(char* password);
    };
}

#endif //TALOS_ENCRYPT_UTIL_H
