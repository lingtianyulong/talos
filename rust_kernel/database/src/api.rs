use crate::db_log;
use std::boxed::Box;
use std::ffi::CStr;
use std::os::raw::{c_char, c_int};
use std::ptr;

use crate::db::DbManager;
use crate::entity::user::{User, CUser};

/*
 * RustDbOpaque is a opaque pointer to the DbManager struct
 * it is used to pass the DbManager struct to the Rust code
 * from the C code.
 * In C++, it can be declared as: struct RustDb
 */
#[repr(C)]
pub struct RustDbOpaque {
    /**
     * 1. 这是一个零大小的字段, 用于创建一个“不透明指针”类型,向 C/C++ 程序隐藏 Rust 内部实现细节
     * 2. 由于字段是私有的（_private），外部代码无法直接创建 RustDbOpaque 的实例，必须通过你提供的 FFI 函数（如 connect_db）来获取指针
     * 3. [u8; 0] 是一个零长度数组，不占用任何内存空间，但让结构体成为一个有效的类型定义
     * 4. 类型安全
     *    实际传递的是 *mut DbManager，但通过类型转换为 *mut RustDbOpaque，可以：
     *    - 在 C/C++ 侧提供类型安全（不能随意转换为其他指针）
     *    - 隐藏 DbManager 的内部结构
     *    - 防止 C/C++ 代码直接访问 Rust 结构体的字段
     */
    _private: [u8; 0],
}

/// Connect to the database
///
/// # Arguments
/// * `conn` - Database connection string (UTF-8 encoded C string)
///
/// # Returns
/// * Non-null pointer to `RustDbOpaque` on success
/// * Null pointer on failure
///
/// # Safety
/// * `conn` must be a valid pointer to a null-terminated C string
/// * `conn` must point to valid UTF-8 data
/// * The returned pointer must be freed with `destroy_db` when done
#[unsafe(no_mangle)]
pub unsafe extern "C" fn connect_db(conn: *const c_char) -> *mut RustDbOpaque {
    if conn.is_null() {
        return ptr::null_mut();
    }
    db_log::log_info("in connect_db...");
    let db_url = unsafe { CStr::from_ptr(conn).to_string_lossy().into_owned() };
    match DbManager::new(&db_url) {
        Ok(db) => {
            db_log::log_info("Database connected successfully");
            let boxed = Box::new(db);
            Box::into_raw(boxed) as *mut RustDbOpaque
        }
        Err(e) => {
            let error = format!("Failed to connect to database: {}", e);
            db_log::log_error(&error);
            ptr::null_mut()
        }
    }
}

/// Check if the database is connected
///
/// # Arguments
/// * `db` - Pointer to database manager
///
/// # Returns
/// * `true` if connected
/// * `false` if not connected or error
///
/// # Safety
/// * `db` must be a valid pointer returned from `connect_db`
/// * `db` must not have been freed with `destroy_db`
#[unsafe(no_mangle)]
pub unsafe extern "C" fn is_connected(db: *mut RustDbOpaque) -> bool {
    if db.is_null() {
        return false;
    }
    let db_manager = unsafe { &mut *(db as *mut DbManager) };
    let result = db_manager.is_connected();
    match result {
        Ok(result) => result,
        Err(e) => {
            println!("Failed to check if the database is connected: {}", e);
            false
        }
    }
}

/// Destroy the database connection
///
/// # Arguments
/// * `db` - Pointer to database manager to destroy
///
/// # Safety
/// * `db` must be a valid pointer returned from `connect_db`
/// * `db` must not have been freed before
/// * `db` must not be used after this call
#[unsafe(no_mangle)]
pub unsafe extern "C" fn destroy_db(db: *mut RustDbOpaque) {
    if db.is_null() {
        return;
    }
    unsafe {
        drop(Box::from_raw(db as *mut DbManager));
    }
}

/// Insert a user into the database
///
/// # Arguments
/// * `db` - Pointer to database manager
/// * `user_name` - User name (UTF-8 encoded C string)
/// * `password` - Password (UTF-8 encoded C string)
///
/// # Returns
/// * Positive integer: user ID on success
/// * -1: failure
///
/// # Safety
/// * `db` must be a valid pointer returned from `connect_db`
/// * `user_name` must be a valid pointer to a null-terminated C string
/// * `password` must be a valid pointer to a null-terminated C string
/// * Both strings must point to valid UTF-8 data
#[unsafe(no_mangle)]
pub unsafe extern "C" fn insert_user(
    db: *mut RustDbOpaque,
    user_name: *const c_char,
    password: *const c_char,
) -> c_int {
    if db.is_null() || user_name.is_null() || password.is_null() {
        return -1;
    }

    let db_manager = unsafe { &mut *(db as *mut DbManager) };
    let name = unsafe { CStr::from_ptr(user_name).to_string_lossy().into_owned() };
    let password = unsafe { CStr::from_ptr(password).to_string_lossy().into_owned() };
    let user = User::new(name, password);
    let result = db_manager.insert_user(&user);
    match result {
        Ok(id) => {
            db_log::log_info(&format!("User inserted successfully with id: {}", id));
            id
        }
        Err(e) => {
            let error = format!("Failed to insert user: {}", e);
            db_log::log_error(&error);
            -1
        }
    }
}

/// Get a user by username
///
/// # Arguments
/// * `db` - Pointer to database manager
/// * `username` - Username to search for (UTF-8 encoded C string)
///
/// # Returns
/// * Non-null pointer to `CUser` on success (must be freed with `destroy_user`)
/// * Null pointer if user not found or error
///
/// # Safety
/// * `db` must be a valid pointer returned from `connect_db`
/// * `username` must be a valid pointer to a null-terminated C string
/// * `username` must point to valid UTF-8 data
/// * The returned pointer must be freed with `destroy_user` when done
#[unsafe(no_mangle)]
pub unsafe extern "C" fn get_user_by_username(
    db: *mut RustDbOpaque,
    username: *const c_char,
) -> *mut CUser {
    if db.is_null() || username.is_null() {
        return ptr::null_mut();
    }

    let db_manager = unsafe { &mut *(db as *mut DbManager) };
    let name = unsafe { CStr::from_ptr(username).to_string_lossy().into_owned() };
    let user = db_manager.get_user_by_username(&name);
    match user {
        Ok(user) => {
            db_log::log_info(&format!("User found: {}", user.user_name));
            // 转换为 C 兼容的结构体
            let c_user = user.to_c_user();
            Box::into_raw(Box::new(c_user))
        }
        Err(e) => {
            let error = format!("Failed to get user by username: {}", e);
            db_log::log_error(&error);
            ptr::null_mut()
        }
    }
}

/// Update a user's password
///
/// # Arguments
/// * `db` - Pointer to database manager
/// * `username` - Username to update (UTF-8 encoded C string)
/// * `password` - New password (UTF-8 encoded C string)
///
/// # Returns
/// * Positive integer: number of affected rows on success
/// * -1: failure
///
/// # Safety
/// * `db` must be a valid pointer returned from `connect_db`
/// * `username` must be a valid pointer to a null-terminated C string
/// * `password` must be a valid pointer to a null-terminated C string
/// * Both strings must point to valid UTF-8 data
#[unsafe(no_mangle)]
pub unsafe extern "C" fn update_user_password(
    db: *mut RustDbOpaque,
    username: *const c_char,
    password: *const c_char,
) -> c_int {
    if db.is_null() || username.is_null() || password.is_null() {  
        return -1;
    }

    let db_manager = unsafe { &mut *(db as *mut DbManager) };
    let name = unsafe { CStr::from_ptr(username).to_string_lossy().into_owned() };
    let pwd = unsafe { CStr::from_ptr(password).to_string_lossy().into_owned() };
    let result = db_manager.update_user_password(&name, &pwd);
    match result {
        Ok(affected_rows) => {
            db_log::log_info(&format!("User password updated successfully with affected rows: {}", affected_rows));
            affected_rows
        }
        Err(e) => {
            let error = format!("Failed to update user password: {}", e);
            db_log::log_error(&error);
            -1
        }
    }
}

/// Delete a user by username
///
/// # Arguments
/// * `db` - Pointer to database manager
/// * `username` - Username to delete (UTF-8 encoded C string)
///
/// # Returns
/// * Positive integer: number of affected rows on success
/// * -1: failure
///
/// # Safety
/// * `db` must be a valid pointer returned from `connect_db`
/// * `username` must be a valid pointer to a null-terminated C string
/// * `username` must point to valid UTF-8 data
/// * The returned pointer must be freed with `destroy_user` when done
#[unsafe(no_mangle)]
pub unsafe extern "C" fn delete_user(
    db: *mut RustDbOpaque,
    username: *const c_char,
) -> c_int {
    if db.is_null() || username.is_null() {
        return -1;  
    }

    let db_manager = unsafe { &mut *(db as *mut DbManager) };
    let name = unsafe { CStr::from_ptr(username).to_string_lossy().into_owned() };
    let result = db_manager.delete_user(&name);
    match result {
        Ok(affected_rows) => {
            db_log::log_info(&format!("User deleted successfully with affected rows: {}", affected_rows));
            affected_rows
        }
        Err(e) => {
            let error = format!("Failed to delete user: {}", e);
            db_log::log_error(&error);
            -1
        }
    }
}

/// Get all users
///
/// # Arguments
/// * `db` - Pointer to database manager
///
/// # Returns
/// * Non-null pointer to `Vec<CUser>` on success (must be freed with `destroy_users`)
/// * Null pointer if error
///
/// # Safety
/// * `db` must be a valid pointer returned from `connect_db`
/// * The returned pointer must be freed with `destroy_users` when done
#[unsafe(no_mangle)]
pub unsafe extern "C" fn get_all_users(
    db: *mut RustDbOpaque,
) -> *mut Vec<CUser> {
    if db.is_null() {
        return ptr::null_mut();
    }

    let db_manager = unsafe { &mut *(db as *mut DbManager) };
    let result = db_manager.get_all_users();
    match result {
        Ok(users) => {
            db_log::log_info(&format!("All users found: {}", users.len()));
            let c_users = users.into_iter().map(|user| user.to_c_user()).collect();
            Box::into_raw(Box::new(c_users))
        }
        Err(e) => {
            let error = format!("Failed to get all users: {}", e);
            db_log::log_error(&error);
            ptr::null_mut()
        }
    }
}