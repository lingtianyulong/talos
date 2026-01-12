//
// Created by lingt on 2025/12/23.
//

#include "user.h"
#include <QJsonDocument>
#include <QJsonObject>

namespace talos::models {
    User::User() : _username(""), _password("") {
    }

    User::User(const std::string &username, const std::string &password)
        : _username(username), _password(password) {
    }

    User::User(const User &user) {
        this->_username = user._username;
        this->_password = user._password;
    }

    User::User(User &&user) noexcept {
        this->_username = std::move(user._username);
        this->_password = std::move(user._password);
    }

    User &User::operator=(const User &user) {
        this->_username = user._username;
        this->_password = user._password;
        return *this;
    }

    User &User::operator=(User &&user) noexcept {
        this->_username = std::move(user._username);
        this->_password = std::move(user._password);
        return *this;
    }

    User::~User() {
    }

    std::string User::to_json() {
        QJsonObject obj;
        obj["user_name"] = QString::fromStdString(this->_username);
        obj["password"] = QString::fromStdString(this->_password);

        QJsonDocument doc(obj);
        QString json = doc.toJson(QJsonDocument::Compact);
        return json.toUtf8().constData();
    }

    std::optional<User> User::from_json(const std::string &json) {
        QString content = QString::fromStdString(json);
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8(), &error);
        if (error.error != QJsonParseError::NoError) {
            return std::nullopt;
        }

        auto obj = doc.object();
        User user;
        if (obj.contains("username")) {
            user._username = obj["username"].toString().toUtf8();
        }

        if (obj.contains("password")) {
            user._password = obj["password"].toString().toUtf8();
        }
        return std::make_optional(user);
    }
}
