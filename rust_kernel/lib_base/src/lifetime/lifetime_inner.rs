use std::sync::{
    Mutex,
    atomic::{AtomicBool, Ordering},
};

pub type LifetimeCallback = Box<dyn FnOnce() + Send + 'static>;

pub struct LifetimeInner {
    pub callbacks: Mutex<Vec<LifetimeCallback>>,
    pub destroyed: AtomicBool,
}

impl LifetimeInner {
    pub fn new() -> Self {
        Self {
            callbacks: Mutex::new(Vec::new()),
            destroyed: AtomicBool::new(false),
        }
    }

    pub fn add<F>(&self, callback: F)
    where
        F: FnOnce() + Send + 'static,
    {
        if self.destroyed.load(Ordering::Acquire) {
            // 已销毁，立即执行，避免泄漏
            callback();
            return;
        }

        let mut callbacks = match self.callbacks.try_lock() {
            Ok(cbs) => cbs,
            Err(_) => {
                // 锁被占用，稍后重试
                eprintln!("LifetimeInner::add: callbacks lock is poisoned");
                self.callbacks.lock().unwrap().push(Box::new(callback));
                return;
            }
        };
        callbacks.push(Box::new(callback));
    }

    pub fn destroy(&self) {
        if self.destroyed.swap(true, Ordering::AcqRel) {
            return; // 已执行过
        }

        let mut callbacks = self.callbacks.lock().unwrap();
        // Vector 的 pop() 默认就是后进先出 (LIFO)
        while let Some(cb) = callbacks.pop() {
            cb();
        }
    }
}

/// 离开作用域时自动调用 destroy()
impl Drop for LifetimeInner {
    fn drop(&mut self) {
        self.destroy();
    }
}
