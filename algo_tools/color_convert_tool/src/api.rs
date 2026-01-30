use crate::tool::*;
use base_tool::abi::*;
use base_tool::glue::create_algo_tool;
use std::ffi::c_char;

pub extern "C" fn name(obj: *mut ToolObject) -> *const c_char {
    unsafe { base_tool::glue::name::<ColorConvertTool>(obj) }
}

pub extern "C" fn init(obj: *mut ToolObject) -> bool {
    unsafe { base_tool::glue::init::<ColorConvertTool>(obj) }
}

pub extern "C" fn run(obj: *mut ToolObject) -> i32 {
    unsafe { base_tool::glue::run::<ColorConvertTool>(obj) }
}

pub extern "C" fn destroy(obj: *mut ToolObject) {
    unsafe { base_tool::glue::destroy::<ColorConvertTool>(obj) }
}

static VTABLE: ToolVTable = ToolVTable {
    name: name,
    init: init,
    run: run,
    destroy: destroy,
};

#[unsafe(no_mangle)]
pub extern "C" fn create_tool() -> Tool {
    let tool = ColorConvertTool::new("颜色转换".to_string());
    create_algo_tool(tool, &VTABLE)
}
