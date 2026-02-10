use crate::lifetime::lifetime_inner;
use std::ffi::c_void;

pub type LifetimeCb = extern "C" fn(*mut c_void);

#[allow(dead_code)]
#[repr(transparent)]
struct SendRawPtr(*mut c_void);
unsafe impl Send for SendRawPtr {}

#[allow(dead_code)]
#[repr(transparent)]
struct SendFnPtr(extern "C" fn(*mut c_void));
unsafe impl Send for SendFnPtr {}

fn wrap_c_callback(
    cb: extern "C" fn(*mut c_void),
    userdata: *mut c_void,
) -> lifetime_inner::LifetimeCallback {
    let userdata_wrapper = SendRawPtr(userdata);
    let cb_wrapper = SendFnPtr(cb);
    Box::new(move || {
        let cb = cb_wrapper;
        let userdata = userdata_wrapper;
        (cb.0)(userdata.0);
    })
}

#[repr(C)]
pub struct Lifetime {
    _private: [u8; 0],
}

#[unsafe(no_mangle)]
pub extern "C" fn lifetime_new() -> *mut Lifetime {
    let inner = lifetime_inner::LifetimeInner::new();
    Box::into_raw(Box::new(inner)) as *mut Lifetime
}

#[unsafe(no_mangle)]
pub extern "C" fn lifetime_add(lifetime: *mut Lifetime, cb: LifetimeCb, userdata: *mut c_void) {
    if lifetime.is_null() {
        return;
    }

    let inner = unsafe { &*(lifetime as *mut lifetime_inner::LifetimeInner) };
    let mut cbs = match inner.callbacks.try_lock() {
        Ok(cbs) => cbs,
        Err(_) => {
            // 锁被占用，稍后重试
            eprintln!("LifetimeInner::add: callbacks lock is poisoned");
            return;
        }
    };
    cbs.push(wrap_c_callback(cb, userdata));
}

/// 释放 lifetime 所管理的对象的资源
#[unsafe(no_mangle)]
pub extern "C" fn lifetime_destroy(lifetime: *mut Lifetime) {
    if lifetime.is_null() {
        return;
    }

    let mut callbacks = {
        let inner = unsafe { &*(lifetime as *mut lifetime_inner::LifetimeInner) };
        let mut cbs = match inner.callbacks.try_lock() {
            Ok(cbs) => cbs,
            Err(_) => {
                // 锁被占用，稍后重试
                eprintln!("LifetimeInner::destroy: callbacks lock is poisoned");
                return;
            }
        };
        // 先通过解析引用得到 vec, 再取出其可变引用 &mut
        // 再通过 take 方法将 vec 的所有权转移出来,并在原来的位置放回一个 Vec::default()
        std::mem::take(&mut (*cbs))
    };

    while let Some(cb) = callbacks.pop() {
        cb();
    }
}

/// 释放 lifetime 结构体本身的内存
#[unsafe(no_mangle)]
pub extern "C" fn lifetime_free(lifetime: *mut Lifetime) {
    if lifetime.is_null() {
        return;
    }

    unsafe {
        let _ = Box::from_raw(lifetime as *mut lifetime_inner::LifetimeInner);
    }
}
