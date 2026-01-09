use std::ffi::CStr;
use std::os::raw::c_char;
use std::path::PathBuf;

use windows::core::link;

mod dump;
mod handler;
mod seh;

use handler::{veh_handler, ueh_handler};
use seh::EXCEPTION_POINTERS;

/* ===== UEH ===== */
type TopLevelFilter =
    unsafe extern "system" fn(*mut EXCEPTION_POINTERS) -> i32;

link!("kernel32.dll" "system" fn SetUnhandledExceptionFilter(
    filter: Option<TopLevelFilter>
) -> Option<TopLevelFilter>);

/* ===== VEH ===== */
type VectoredHandler =
    unsafe extern "system" fn(*mut EXCEPTION_POINTERS) -> i32;

link!("kernel32.dll" "system" fn AddVectoredExceptionHandler(
    first: u32,
    handler: Option<VectoredHandler>
) -> *mut core::ffi::c_void);

#[unsafe(no_mangle)]
pub extern "C" fn init_dump(dump_dir: *const c_char) -> bool {
    if dump_dir.is_null() {
        return false;
    }

    let dir = unsafe { CStr::from_ptr(dump_dir) }
        .to_str()
        .ok()
        .map(PathBuf::from)
        .unwrap();

    handler::set_dump_dir(dir);

    unsafe {
        // VEH 优先
        AddVectoredExceptionHandler(1, Some(veh_handler));
        // UEH 兜底
        SetUnhandledExceptionFilter(Some(ueh_handler));
    }

    true
}
