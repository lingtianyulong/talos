/**
 * User entity
 * @author: talos
 * @date: 2025-11-27
 * @description: 对数据库中用到的 用户实体定义, 封装后, 在 Qt 中进行调用,
 *               将底层数据库操作封装为 C 语言接口, 方便在 Qt 中进行调用,
 *               实现用户登录、注册、注销等功能, 但功能逻辑, 使用 rust 实现
 * @version: 1.0.0
 */
use std::ffi::{CStr, CString};
use std::os::raw::c_char;

/// Rust 内部使用的 User 结构体
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub struct User {
    pub user_name: String,
    pub password: String,
}

impl User {
    pub fn new(user_name: String, password: String) -> Self {
        Self {
            user_name,
            password,
        }
    }
    
    /// 转换为 C 兼容的结构体
    pub fn to_c_user(&self) -> CUser {
        CUser::new(&self.user_name, &self.password)
    }
}


/// C 兼容的 User 结构体，用于 FFI
/// 使用 user 进行内部交互, 防止 FFI 层直接访问 Rust 内部结构体, 避免对 Rust 内部数据造成污染
/// 
/// # Memory Layout
/// 在 C/C++ 中可以定义为:
/// ```c
/// struct CUser {
///     char* user_name;
///     char* password;
/// };
/// ```
#[repr(C)]
pub struct CUser {
    pub user_name: *mut c_char,
    pub password: *mut c_char,
}

impl CUser {
    /// 从 Rust 字符串创建 CUser
    pub fn new(user_name: &str, password: &str) -> Self {
        let user_name = CString::new(user_name).unwrap_or_default();
        let password = CString::new(password).unwrap_or_default();
        
        Self {
            user_name: user_name.into_raw(),
            password: password.into_raw(),
        }
    }
}

/// Create a new CUser instance and return an owned pointer
///
/// # Arguments
/// * `user_name` - Username (UTF-8 encoded C string)
/// * `password` - Password (UTF-8 encoded C string)
///
/// # Returns
/// * Non-null pointer to a heap-allocated `CUser` instance
///
/// # Safety
/// * `user_name` must be a valid pointer to a null-terminated C string
/// * `password` must be a valid pointer to a null-terminated C string
/// * Both strings must point to valid UTF-8 data
/// * The returned pointer must be freed with `destroy_user` when done
#[unsafe(no_mangle)]
pub unsafe extern "C" fn create_user(
    user_name: *const c_char,
    password: *const c_char,
) -> *mut CUser {
    let user_name = unsafe { CStr::from_ptr(user_name).to_string_lossy() };
    let password = unsafe { CStr::from_ptr(password).to_string_lossy() };
    Box::into_raw(Box::new(CUser::new(&user_name, &password)))
}

/// Destroy a CUser instance and free its memory
///
/// # Arguments
/// * `user` - Pointer to a CUser instance to destroy
///
/// # Safety
/// * `user` must be a valid pointer returned from `create_user` or `get_user_by_username`
/// * `user` must not have been freed before
/// * `user` must not be used after this call
#[unsafe(no_mangle)]
pub unsafe extern "C" fn destroy_user(user: *mut CUser) {
    if user.is_null() {
        return;
    }
    unsafe {
        let user = Box::from_raw(user);
        // 释放内部的 CString 内存
        if !user.user_name.is_null() {
            let _ = CString::from_raw(user.user_name);
        }
        if !user.password.is_null() {
            let _ = CString::from_raw(user.password);
        }
        // user (Box) 会在这里自动 drop
    }
}
