//
// Created by lingt on 2025/12/24.
//

#ifndef TALOS_API_CONFIG_H
#define TALOS_API_CONFIG_H

#include <string>
#include <format>

namespace talos::url {
#ifdef BUILD_DEBUG
    constexpr std::string_view BASE_URL = "http://127.0.0.1:8080/api/v1";
    constexpr std::string_view LOGIN = "/user/login";
    constexpr std::string_view REGISTER = "/user/register";
#elif defined(BUILD_RELEASE)
    constexpr std::string_view Url = "http://127.0.0.1:8080/api/v1";
#endif
}

#endif //TALOS_API_CONFIG_H
