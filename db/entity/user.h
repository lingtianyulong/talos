//
// Created by lingt on 2025/11/28.
//

#ifndef TALOS_USER_H
#define TALOS_USER_H

#include "../rust/user_rust.h"

#include <memory>
#include <string>
#include <string_view>

namespace talos::db {

class User {
public:
    User();
    User(std::string_view user_name, std::string_view password);
    User(const User& user);
    User(User&& user) noexcept;
    User& operator=(const User& user);
    User& operator=(User&& user) noexcept;
    ~User();

    RustUser* getRustUser() const;

    std::string getPassword() const;
    std::string getUserName() const;

private:
    std::unique_ptr<RustUser> _user = std::make_unique<RustUser>();
};

class UserManager {
public:
    static int insertUser(const User& user);
    static User getUserByUserName(std::string_view user_name);
};

}  // namespace talos::db

#endif  // TALOS_USER_H
