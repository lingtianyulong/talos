//
// Created by lingt on 2025/11/29.
//

#ifndef TALOS_USER_RUST_H
#define TALOS_USER_RUST_H

#include "db_rust.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CUser {
    const char *user_name;
    const char *password;
} RustUser;

int insert_user(RustDb *db, const char *user_name, const char *password);

RustUser *get_user_by_username(RustDb *db, const char *user_name);

#ifdef __cplusplus
}
#endif

#endif //TALOS_USER_RUST_H
