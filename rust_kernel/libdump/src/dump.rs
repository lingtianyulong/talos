use std::ffi::c_void;
use std::fs::File;
use std::os::windows::io::AsRawHandle;
use std::path::PathBuf;

use windows::Win32::{
    Foundation::*,
    System::{
        Diagnostics::Debug::*,
        Threading::*,
    },
};

use crate::seh::EXCEPTION_POINTERS;

pub unsafe fn write_dump(
    info: *mut EXCEPTION_POINTERS,
    path: PathBuf,
) {
    let file = match File::create(path) {
        Ok(f) => f,
        Err(_) => return,
    };

    unsafe {
        let mut mei = MINIDUMP_EXCEPTION_INFORMATION {
            ThreadId: GetCurrentThreadId(),
            ExceptionPointers: info as *mut _,
            ClientPointers: false.into(),
        };

        let _ = MiniDumpWriteDump(
            GetCurrentProcess(),
            GetCurrentProcessId(),
            HANDLE(file.as_raw_handle() as *mut c_void),
            MiniDumpWithFullMemory,
            Some(&mut mei),
            None,
            None,
        );
    }
}
