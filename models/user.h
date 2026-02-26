//
// Created by lingt on 2025/12/23.
//

#ifndef TALOS_USER_H
#define TALOS_USER_H

#include <optional>
#include <string>

namespace talos::models {
class User {
public:
    User();

    User(const std::string& username, const std::string& password);

    explicit User(const User& user);

    explicit User(User&& user) noexcept;

    User& operator=(const User& user);

    User& operator=(User&& user) noexcept;

    ~User();

    std::string to_json();

    std::optional<User> from_json(const std::string& json);

private:
    std::string _username;
    std::string _password;
};
}  // namespace talos::models
#endif  // TALOS_USER_H
