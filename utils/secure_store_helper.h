//
// Created by lingt on 2025/12/20.
//

#ifndef TALOS_SECURE_STORE_HELPER_H
#define TALOS_SECURE_STORE_HELPER_H

namespace utils {
#ifdef __cplusplus
    extern "C" {
#endif

    /// 设置密钥值
    /// # Arguments
    /// * `service` - 服务名称
    /// * `key` - 密钥名称
    /// * `value` - 密钥值
    /// # Returns
    /// * `0` - 成功
    /// * `1` - 密钥不存在
    /// * `2` - 权限不足
    /// * `3` - 后端错误
    /// * `100` - 无效参数
    int secure_store_set(
        const char *service,
        const char *key,
        const char *value
    );

    /// 获取密钥值
    /// # Arguments
    /// * `service` - 服务名称
    /// * `key` - 密钥名称
    /// # Returns
    /// * `0` - 成功
    /// * `1` - 密钥不存在
    /// * `2` - 权限不足
    /// * `3` - 后端错误
    /// * `100` - 无效参数
    char *secure_store_get(
        const char *service,
        const char *key
    );

    /// 删除密钥值
    /// # Arguments
    /// * `service` - 服务名称
    /// * `key` - 密钥名称
    /// # Returns
    /// * `0` - 成功
    /// * `1` - 密钥不存在
    /// * `2` - 权限不足
    /// * `3` - 后端错误
    /// * `100` - 无效参数
    int secure_store_delete(
        const char *service,
        const char *key);

    /// 释放内存
    /// # Arguments
    /// * `ptr` -  secure_store_get 获取到的 secure_store_set 存储的 token 字符串
    /// # Returns
    /// * `0` - 成功
    /// * `1` - 密钥不存在
    /// * `2` - 权限不足
    /// * `3` - 后端错误
    /// * `100` - 无效参数
    void secure_store_free(char *token);

#ifdef __cplusplus
    }
#endif
}


#endif //TALOS_SECURE_STORE_HELPER_H
