use crate::log::init_log;
use std::ffi::CStr;
use std::os::raw::c_char;

#[unsafe(no_mangle)]
pub extern "C" fn init_logger(cfg_file: *const c_char) -> bool {
    if cfg_file.is_null() {
        return false;
    }
    let cfg_file = unsafe { CStr::from_ptr(cfg_file).to_string_lossy().into_owned() };
    match init_log(&cfg_file) {
        Ok(res) => res,
        Err(e) => {
            println!("Failed to initialize logger: {}", e);
            return false;
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn logger_trace(msg: *const c_char) {
    if msg.is_null() {
        return;
    }
    let msg = unsafe { CStr::from_ptr(msg).to_string_lossy().into_owned() };
    crate::log::trace(&msg);
}

#[unsafe(no_mangle)]
pub extern "C" fn logger_debug(msg: *const c_char) {
    if msg.is_null() {
        return;
    }
    let msg = unsafe { CStr::from_ptr(msg).to_string_lossy().into_owned() };
    crate::log::debug(&msg);
}

#[unsafe(no_mangle)]
pub extern "C" fn logger_info(msg: *const c_char) {
    if msg.is_null() {
        return;
    }
    let msg = unsafe { CStr::from_ptr(msg).to_string_lossy().into_owned() };
    crate::log::info(&msg);
}

#[unsafe(no_mangle)]
pub extern "C" fn logger_warn(msg: *const c_char) {
    if msg.is_null() {
        return;
    }
    let msg = unsafe { CStr::from_ptr(msg).to_string_lossy().into_owned() };
    crate::log::warn(&msg);
}

#[unsafe(no_mangle)]
pub extern "C" fn logger_error(msg: *const c_char) {
    if msg.is_null() {
        return;
    }
    let msg = unsafe { CStr::from_ptr(msg).to_string_lossy().into_owned() };
    crate::log::error(&msg);
}

#[unsafe(no_mangle)]
pub extern "C" fn logger_flush() {
    crate::log::flush();
}

#[unsafe(no_mangle)]
pub extern "C" fn shutdown_logger() {
    crate::log::shutdown_log();
}
