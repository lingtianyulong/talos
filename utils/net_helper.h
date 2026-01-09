//
// Created by lingt on 2025/12/22.
//

#ifndef TALOS_NET_HELPER_H
#define TALOS_NET_HELPER_H

namespace utils {
#ifdef __cplusplus
    extern "C" {
#endif
    // post 方式, 访问 url, auth 为登录接口返回的 token,若无 token 传空字符串即可
    char *http_post(const char *url, const char *body, const char *auth = "");

    void free_http_response(char *post);

#ifdef __cplusplus
    }

#endif
}

#endif //TALOS_NET_HELPER_H
