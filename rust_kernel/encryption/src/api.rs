use crate::encrypt_factory::{create_encrypt, EncryptType};
use std::ffi::{ CStr, CString };
use std::os::raw::c_char;
use std::ptr;

#[unsafe(no_mangle)]
pub unsafe extern "C" fn encrypt_password_argon2(password: *const c_char) -> *const c_char {
    let password = unsafe { CStr::from_ptr(password).to_string_lossy().into_owned() };
    let encrypted = match create_encrypt(EncryptType::Argon2).encrypt(password.as_str()) {
        Ok(encrypted) => encrypted,
        Err(e) => {
            println!("Failed to encrypt password: {}", e);
            return ptr::null_mut();
        }
    };
    CString::new(encrypted.as_str()).unwrap().into_raw()
}

/// 释放内存
#[unsafe(no_mangle)]
pub unsafe extern "C" fn release_encrypted_password(encrypted: *mut c_char) {
    if encrypted.is_null() {
        return;
    }
    unsafe {
        drop(CString::from_raw(encrypted));
    }
}



#[unsafe(no_mangle)]
pub unsafe extern "C" fn verify_password_argon2(password: *const c_char, store_password: *const c_char) -> bool {
    let password = unsafe { CStr::from_ptr(password).to_string_lossy().into_owned() };
    let store_password = unsafe { CStr::from_ptr(store_password).to_string_lossy().into_owned() };
    let success = match create_encrypt(EncryptType::Argon2).verify(password.as_str(), store_password.as_str()) {
        Ok(verified) => verified,
        Err(e) => {
            println!("Failed to verify password: {}", e);
            return false;
        }
    };
    success
}