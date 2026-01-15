use crate::abi::*;
use crate::tool_trait::ToolTrait;
use std::ffi::CString;
use std::os::raw::c_char;

/// 工具包装器
/// 作用
/// ToolWrapper 实际上定义了那些“不透明指针”在 Rust 内存中的真实布局。
/// 它包裹了具体的 Rust 实现 T（实现了 ToolTrait 的类型），
/// 使得 C 代码可以通过统一的指针调用不同 Rust 类型的逻辑
pub struct ToolWrapper<T: ToolTrait> {
    pub inner: T,      // 通过类型擦除, 保留具体的 Rust 实现 T（实现了 ToolTrait 的类型）
    pub name: CString, // 工具名称
}

pub unsafe extern "C" fn name<T: ToolTrait>(obj: *mut ToolObject) -> *const c_char {
    unsafe {
        let wrapper = &mut *(obj as *mut ToolWrapper<T>);
        wrapper.name.as_ptr()
    }
}

pub unsafe extern "C" fn init<T: ToolTrait>(obj: *mut ToolObject) -> bool {
    unsafe {
        let wrapper = &mut *(obj as *mut ToolWrapper<T>);
        wrapper.inner.init()
    }
}

pub unsafe extern "C" fn run<T: ToolTrait>(obj: *mut ToolObject) -> i32 {
    unsafe {
        let wrapper = &mut *(obj as *mut ToolWrapper<T>);
        wrapper.inner.run()
    }
}

pub unsafe extern "C" fn destroy<T: ToolTrait>(obj: *mut ToolObject) {
    if !obj.is_null() {
        unsafe {
            // 重新构建 Box，这会接管原始指针的所有权
            let wrapper = Box::from_raw(obj as *mut ToolWrapper<T>);
            wrapper.inner.destroy();
        }
    }
}

pub fn create_algo_tool<T: ToolTrait + 'static>(tool: T, vtable: &'static ToolVTable) -> Tool {
    let name = CString::new(tool.name()).unwrap();
    let wrapper = Box::new(ToolWrapper { inner: tool, name });

    Tool {
        obj: Box::into_raw(wrapper) as *mut ToolObject,
        vtable,
    }
}
