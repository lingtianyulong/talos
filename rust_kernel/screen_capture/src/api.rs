use crate::captuer;
use crate::log;

use std::ptr;

#[unsafe(no_mangle)]
pub extern "C" fn init_capture(width: usize, height: usize) -> bool {
    match captuer::init(width, height) {
        Ok(_) => true,
        Err(e) => {
            log::error(&format!("Failed to init capture: {}", e).as_str());
            false
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn start_capture() -> bool {
    match captuer::start_capture() {
        Ok(_) => true,
        Err(e) => {
            log::error(&format!("Failed to start capture: {}", e).as_str());
            false
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn get_frame_buffer() -> *const u8 {
    match captuer::get_frame_buffer() {
        Ok(frame_buffer) => frame_buffer.as_ptr(),
        Err(e) => {
            log::error(&format!("Failed to get frame buffer: {}", e).as_str());
            return ptr::null();
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn stop_capture() -> bool {
    match captuer::stop_capture() {
        Ok(_) => true,
        Err(e) => {
            log::error(&format!("Failed to stop capture: {}", e).as_str());
            false
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn is_frame_ready() -> bool {
    captuer::is_frame_ready()
}
