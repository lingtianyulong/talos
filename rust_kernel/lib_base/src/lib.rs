pub mod lifetime;
pub use lifetime::api::*;

#[cfg(test)]
mod tests {
    use std::ffi::c_void;
    use std::sync::Arc;
    use std::sync::atomic::{AtomicU32, Ordering};

    #[test]
    fn test_lifetime_basic() {
        use crate::lifetime::api::*;

        static COUNTER: AtomicU32 = AtomicU32::new(0);

        extern "C" fn callback(userdata: *mut c_void) {
            let expected_val = userdata as usize as u32;
            COUNTER.fetch_add(expected_val, Ordering::SeqCst);
        }

        let lt = lifetime_new();
        assert!(!lt.is_null());

        // 注册两个回调
        lifetime_add(lt, callback, 10 as *mut c_void);
        lifetime_add(lt, callback, 20 as *mut c_void);

        assert_eq!(COUNTER.load(Ordering::SeqCst), 0);

        // 销毁并触发回调
        lifetime_destroy(lt);

        assert_eq!(COUNTER.load(Ordering::SeqCst), 30);
    }

    #[test]
    fn test_lifetime_lifo_order() {
        use crate::lifetime::api::*;

        let lt = lifetime_new();

        let results = Arc::new(std::sync::Mutex::new(Vec::<u32>::new()));

        let results_ptr = Arc::into_raw(results.clone()) as *mut c_void;

        extern "C" fn callback(userdata: *mut c_void) {
            let val = (userdata as usize & 0xFF) as u32;
            let results_ptr = (userdata as usize >> 8) as *mut std::sync::Mutex<Vec<u32>>;
            let results = unsafe { &*results_ptr };
            results.lock().unwrap().push(val);
        }

        // 构造 userdata: 高位是 results 指针，低位是值
        let make_userdata = |val: usize, ptr: *mut c_void| -> *mut c_void {
            (((ptr as usize) << 8) | (val & 0xFF)) as *mut c_void
        };

        lifetime_add(lt, callback, make_userdata(1, results_ptr));
        lifetime_add(lt, callback, make_userdata(2, results_ptr));
        lifetime_add(lt, callback, make_userdata(3, results_ptr));

        lifetime_destroy(lt);

        let final_results = results.lock().unwrap();
        // 验证 LIFO 顺序: 3, 2, 1
        assert_eq!(*final_results, vec![3, 2, 1]);

        // 清理 Arc
        unsafe { Arc::from_raw(results_ptr as *mut std::sync::Mutex<Vec<u32>>) };
    }
}
