//
// Created by lingt on 2025/11/28.
//

#include "user.h"

#include "db_util.h"

#include <cstring>
#include <memory>

#ifdef _MSC_VER
#define strdup _strdup
#endif

namespace talos::db {
User::User() {
    if (!_user) {
        _user = std::make_unique<RustUser>();
    }
}

User::User(std::string_view user_name, std::string_view password) {
    if (!_user) {
        _user = std::make_unique<RustUser>();
    }

    if (!user_name.empty() && !password.empty()) {
        std::string user_name_copy(user_name);
        std::string user_password_copy(password);
        _user->user_name = strdup(user_name_copy.c_str());
        _user->password = strdup(user_password_copy.c_str());
    }
}

User::User(const User& user) {
    // 只能进行深拷贝, 否则, this->_user 与 user._user 会占用相同的内存, 当析构时, 会引起 double
    // free.
    if (user._user) {
        _user = std::make_unique<RustUser>(*user._user);
    }
}

User::User(User&& user) noexcept {
    // 此处移动后, 原对象不再拥有所有权
    _user = std::move(user._user);
}

User& User::operator=(const User& user) {
    if (this == &user) {
        return *this;
    }

    if (user._user) {
        _user = std::make_unique<RustUser>(*user._user);
    }
    return *this;
}

User& User::operator=(User&& user) noexcept {
    _user = std::move(user._user);
    return *this;
}

User::~User() {
    if (_user) {
        if (_user->user_name) {
            free(const_cast<char*>(_user->user_name));
        }
        if (_user->password) {
            free(const_cast<char*>(_user->password));
        }

        _user.reset();
    }
}

RustUser* User::getRustUser() const {
    return _user.get();
}

std::string User::getPassword() const {
    if (!_user) {
        return "";
    }
    return _user->password;
}
std::string User::getUserName() const {
    if (!_user) {
        return "";
    }
    return _user->user_name;
}

int UserManager::insertUser(const User& user) {
    if (DbUtil::instance().raw() == nullptr) {
        return -1;
    }
    return insert_user(DbUtil::instance().raw(), user.getRustUser()->user_name,
                       user.getRustUser()->password);
}

User UserManager::getUserByUserName(string_view user_name) {
    auto db = DbUtil::instance().raw();
    if (RustUser* rust_user = get_user_by_username(db, user_name.data())) {
        User user(rust_user->user_name, rust_user->password);
        return user;
    }
    return User();
}
}  // namespace talos::db
