#[repr(C)]
pub struct ToolObject {
    _private: [u8; 0],
}

#[repr(C)]
pub struct ToolVTable {
    pub name: extern "C" fn(obj: *mut ToolObject) -> *const std::ffi::c_char,
    pub init: extern "C" fn(obj: *mut ToolObject) -> bool,
    pub run: extern "C" fn(obj: *mut ToolObject) -> i32,
    pub destroy: extern "C" fn(obj: *mut ToolObject),
}

#[repr(C)]
pub struct Tool {
    pub vtable: *const ToolVTable,
    pub obj: *mut ToolObject,
}
