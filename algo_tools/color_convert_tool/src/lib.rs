pub mod api;
pub mod tool;
pub mod tool_logger;

#[allow(unused_imports)]
use crate::api::*;

pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::ffi::CStr;
    use std::ffi::c_char;

    #[test]
    fn test_color_convert_tool() {
        let tool = create_tool();
        let name_fn = unsafe { &(*tool.vtable).name };
        let name = unsafe { CStr::from_ptr(name_fn(tool.obj) as *const c_char) }
            .to_str()
            .unwrap();
        assert_eq!(name, "Color Convert Tool");

        let init = unsafe { ((*tool.vtable).init)(tool.obj) };
        assert_eq!(init, true);

        let run = unsafe { ((*tool.vtable).run)(tool.obj) };
        assert_eq!(run, 0);

        let destroy = unsafe { ((*tool.vtable).destroy)(tool.obj) };
        assert_eq!(destroy, ());
    }
}
