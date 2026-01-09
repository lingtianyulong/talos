use windows::Win32::System::Diagnostics::Debug::{
    CONTEXT,
    EXCEPTION_RECORD,
};

#[repr(C)]
#[allow(non_camel_case_types)]
#[allow(non_snake_case)]
pub struct EXCEPTION_POINTERS {
    pub ExceptionRecord: *mut EXCEPTION_RECORD,
    pub ContextRecord: *mut CONTEXT,
}
