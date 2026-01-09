use std::path::PathBuf;
use std::sync::atomic::{AtomicBool, Ordering};
use once_cell::sync::OnceCell;
use windows::Win32::System::Diagnostics::Debug::EXCEPTION_CONTINUE_SEARCH;

use crate::{dump::write_dump, seh::EXCEPTION_POINTERS};

// static mut DUMP_DIR: Option<PathBuf> = None;
static DUMP_DIR: OnceCell<PathBuf> = OnceCell::new();
static DUMPED: AtomicBool = AtomicBool::new(false);

fn make_dump_path() -> PathBuf {
    let mut dir = DUMP_DIR.get().expect("DUMP_DIR is not initialized").clone();
    let pid = unsafe { windows::Win32::System::Threading::GetCurrentProcessId() };
    let ts = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .unwrap()
        .as_secs();
    dir.push(format!("crash_{}_{}.dmp", pid, ts));
    dir
}

/* ================= VEH ================= */

pub unsafe extern "system" fn veh_handler(
    info: *mut EXCEPTION_POINTERS,
) -> i32 {
    if !DUMPED.swap(true, Ordering::SeqCst) {
        unsafe {
            let path = make_dump_path();
            write_dump(info, path);
        }
    }

    EXCEPTION_CONTINUE_SEARCH as i32
}

/* ================= UEH ================= */

pub unsafe extern "system" fn ueh_handler(
    info: *mut EXCEPTION_POINTERS,
) -> i32 {
    if !DUMPED.swap(true, Ordering::SeqCst) {
        unsafe {
            let path = make_dump_path();
            write_dump(info, path);
        }
    }

    1 // EXCEPTION_EXECUTE_HANDLER
}

pub fn set_dump_dir(dir: PathBuf) {
    DUMP_DIR.set(dir).expect("Failed to set DUMP_DIR");
}
