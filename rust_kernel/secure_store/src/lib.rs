pub mod error;
pub mod store;

use crate::error::SecureStoreError;
use crate::store::SecureStore;

use std::{
    ffi::{CStr, CString},
    os::raw::c_char,
};

const OK: i32 = 0;
const ERR_NOT_FOUND: i32 = 1;
const ERR_PERMISSION: i32 = 2;
const ERR_BACKEND: i32 = 3;
const ERR_INVALID_ARG: i32 = 100;

/// 内部工具
fn cstr_to_str<'a>(ptr: *const c_char) -> Result<&'a str, i32> {
    if ptr.is_null() {
        return Err(ERR_INVALID_ARG);
    }
    let cstr = unsafe { CStr::from_ptr(ptr) };
    let str = cstr.to_str().map_err(|_| ERR_INVALID_ARG)?;
    Ok(str)
}

fn map_error(err: SecureStoreError) -> i32 {
    match err {
        SecureStoreError::NotFound => ERR_NOT_FOUND,
        SecureStoreError::PermissionDenied => ERR_PERMISSION,
        SecureStoreError::Backend(_) => ERR_BACKEND,
    }
}

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
#[unsafe(no_mangle)]
pub unsafe extern "C" fn secure_store_set(
    service: *const c_char,
    key: *const c_char,
    value: *const c_char,
) -> i32 {
    let service = match cstr_to_str(service) {
        Ok(srv) => srv,
        Err(e) => return e,
    };

    let key = match cstr_to_str(key) {
        Ok(k) => k,
        Err(e) => return e,
    };

    let value = match cstr_to_str(value) {
        Ok(v) => v,
        Err(e) => return e,
    };

    let store = SecureStore::new(service);
    match store.set(key, value) {
        Ok(_) => OK,
        Err(e) => map_error(e),
    }
}

/// 获取密钥值
/// # Arguments
/// * `service` - 服务名称
/// * `key` - 密钥名称
/// # Returns
///   返回错误描述及错误码, 错误码定义如下:
/// * `0` - 成功
/// * `1` - 密钥不存在
/// * `2` - 权限不足
/// * `3` - 后端错误
/// * `100` - 无效参数
#[unsafe(no_mangle)]
pub unsafe extern "C" fn secure_store_get(
    service: *const c_char,
    key: *const c_char,
) -> *mut c_char {
    let service = match cstr_to_str(service) {
        Ok(srv) => srv,
        Err(e) => {
            let info = format!(
                "secure_store_get method failed to get service parameter, error code: {}",
                e
            );
            return CString::new(info).unwrap().into_raw();
        }
    };

    let key = match cstr_to_str(key) {
        Ok(k) => k,
        Err(e) => {
            let info = format!(
                "secure_store_get method failed to get key parameter, error code: {}",
                e
            );
            return CString::new(info).unwrap().into_raw();
        }
    };

    let store = SecureStore::new(service);
    match store.get(key) {
        Ok(v) => CString::new(v).unwrap().into_raw(),
        Err(e) => {
            let info = format!(
                "secure_store_get method failed to get value, error code: {}",
                e
            );
            return CString::new(info).unwrap().into_raw();
        }
    }
}

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
#[unsafe(no_mangle)]
pub unsafe extern "C" fn secure_store_delete(service: *const c_char, key: *const c_char) -> i32 {
    let service = match cstr_to_str(service) {
        Ok(v) => v,
        Err(e) => return e,
    };
    let key = match cstr_to_str(key) {
        Ok(v) => v,
        Err(e) => return e,
    };

    let store = SecureStore::new(service);

    match store.delete(key) {
        Ok(_) => OK,
        Err(e) => map_error(e),
    }
}

/// 释放内存
/// # Arguments
/// * `ptr` -  secure_store_get 获取到的 secure_store_set 存储的 token 字符串
/// # Returns
/// * `0` - 成功
/// * `1` - 密钥不存在
/// * `2` - 权限不足
/// * `3` - 后端错误
/// * `100` - 无效参数
#[unsafe(no_mangle)]
pub unsafe extern "C" fn secure_store_free(ptr: *mut c_char) {
    if !ptr.is_null() {
        unsafe {
            drop(CString::from_raw(ptr));
        }
    }
}

#[cfg(test)]
mod tests {
    use crate::store::SecureStore;

    #[test]
    fn test_secure_store_set_get() {
        let service = "test_service";
        let key = "test_key";
        let value = "test_value";

        let store = SecureStore::new(service);

        let res = store.set(key, value);
        assert_eq!(res.is_ok(), true, "set value should be ok");

        let res = store.get(key);
        println!("res: {:?}", res);
        assert_eq!(res.is_ok(), true, "get value should be ok");
        assert_eq!(res.unwrap(), value, "get value should be equal to set value");

        let result = store.delete(key);
        assert_eq!(result.is_ok(), true, "delete value should be ok");
    }
}
