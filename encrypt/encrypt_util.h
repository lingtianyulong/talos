//
// Created by lingt on 2025/12/3.
//

#ifndef TALOS_ENCRYPT_UTIL_H
#define TALOS_ENCRYPT_UTIL_H

#include "rust/encrypt_rust.h"

#include <gsl/gsl>

namespace talos::encrypt {
class EncryptUtil {
    EncryptUtil() = default;

public:
    ~EncryptUtil() = default;
    [[nodiscard]] static const char* encrypt(gsl::not_null<const char*> password);
    [[nodiscard]] static bool verify(gsl::not_null<const char*> password,
                                     gsl::not_null<const char*> storedPwd);
    static void release(gsl::not_null<char*> password);
};

}  // namespace talos::encrypt

#endif  // TALOS_ENCRYPT_UTIL_H
