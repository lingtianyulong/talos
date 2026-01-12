use std::ffi::{CString, c_char};

#[link(name = "logger", kind = "dylib")]
#[allow(unused)]
unsafe extern "C" {
    fn logger_info(msg: *const c_char);
    fn logger_error(msg: *const c_char);
    fn logger_debug(msg: *const c_char);
}

pub fn info(msg: &str) {
    if let Ok(c_msg) = CString::new(msg) {
        unsafe { logger_info(c_msg.as_ptr()) }
    }
}

pub fn error(msg: &str) {
    if let Ok(c_msg) = CString::new(msg) {
        unsafe { logger_error(c_msg.as_ptr()) }
    }
}

#[allow(unused)]
pub fn debug(msg: &str) {
    if let Ok(c_msg) = CString::new(msg) {
        unsafe { logger_debug(c_msg.as_ptr()) }
    }
}
