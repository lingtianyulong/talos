use crate::log;
use std::error::Error;
use std::sync::{
    Arc, Mutex, OnceLock,
    atomic::{AtomicBool, Ordering},
};

use windows::Win32::Graphics::Gdi::*;
use windows::Win32::UI::WindowsAndMessaging::*;

struct FrameState {
    width: usize,
    height: usize,
    frame_buffer: Vec<u8>,   // 后台缓冲区（FFmpeg 写入）
    display_buffer: Vec<u8>, // 显示缓冲区（FFI 读取）
}

impl FrameState {
    fn new(width: usize, height: usize) -> Self {
        Self {
            width,
            height,
            frame_buffer: vec![0; width * height * 3],
            display_buffer: vec![0; width * height * 3],
        }
    }
}

static FRAME_STATE: OnceLock<Arc<Mutex<FrameState>>> = OnceLock::new();
static CAPTURE_RUNNING: AtomicBool = AtomicBool::new(false);
static FIRST_FRAME_READY: AtomicBool = AtomicBool::new(false);

pub fn init(width: usize, height: usize) -> Result<bool, Box<dyn Error>> {
    let frame_state = FrameState::new(width, height);
    match FRAME_STATE.set(Arc::new(Mutex::new(frame_state))) {
        Ok(_) => Ok(true),
        Err(_) => {
            return Err("Failed to init frame state: already initialized".into());
        }
    }
}

fn capture_screen_gdi(
    width: usize,
    height: usize,
    buffer: &mut [u8],
) -> Result<(), Box<dyn Error + Send + Sync>> {
    unsafe {
        let hdc_screen = GetDC(None);
        if hdc_screen.0.is_null() {
            return Err("GetDC failed".into());
        }

        let hdc_mem = CreateCompatibleDC(Some(hdc_screen));
        let hbitmap = CreateCompatibleBitmap(hdc_screen, width as i32, height as i32);
        SelectObject(hdc_mem, hbitmap.into());

        // 屏幕 → 内存 DC
        let _ = BitBlt(
            hdc_mem,
            0,
            0,
            width as i32,
            height as i32,
            Some(hdc_screen),
            0,
            0,
            SRCCOPY,
        );

        let mut bmi = BITMAPINFO {
            bmiHeader: BITMAPINFOHEADER {
                biSize: std::mem::size_of::<BITMAPINFOHEADER>() as u32,
                biWidth: width as i32,
                biHeight: -(height as i32), // top-down
                biPlanes: 1,
                biBitCount: 24,
                biCompression: BI_RGB.0 as u32,
                ..Default::default()
            },
            ..Default::default()
        };

        // 2️⃣ 绘制光标到 framebuffer
        draw_cursor(hdc_mem);

        let ret = GetDIBits(
            hdc_mem,
            hbitmap,
            0,
            height as u32,
            Some(buffer.as_mut_ptr() as *mut _),
            &mut bmi,
            DIB_RGB_COLORS,
        );

        let _ = DeleteObject(hbitmap.into());
        let _ = DeleteDC(hdc_mem);
        ReleaseDC(None, hdc_screen);

        if ret == 0 {
            return Err("GetDIBits failed".into());
        }

        Ok(())
    }
}

#[inline]
unsafe fn draw_cursor(hdc_mem: HDC) {
    unsafe {
        // 1. 获取当前光标信息
        let mut cursor_info = CURSORINFO {
            cbSize: std::mem::size_of::<CURSORINFO>() as u32,
            ..Default::default()
        };

        if !GetCursorInfo(&mut cursor_info).is_ok() {
            return;
        }

        // 光标不可见（如正在隐藏）直接返回
        if cursor_info.flags != CURSOR_SHOWING {
            return;
        }

        // 2. 获取光标 ICON 信息（用于 hotspot）
        let mut icon_info = ICONINFO::default();
        if !GetIconInfo(cursor_info.hCursor.into(), &mut icon_info).is_ok() {
            return;
        }

        // 3. hotspot 修正后的绘制位置
        let x = cursor_info.ptScreenPos.x - icon_info.xHotspot as i32;
        let y = cursor_info.ptScreenPos.y - icon_info.yHotspot as i32;

        // 4. 绘制光标到内存 DC
        //    注意：一定要在 GetDIBits 之前调用
        let _ = DrawIconEx(
            hdc_mem,
            x,
            y,
            cursor_info.hCursor.into(),
            0,
            0,
            0,
            None,
            DI_NORMAL,
        );

        // 5. 释放 GDI 资源（非常重要）
        if !icon_info.hbmColor.is_invalid() {
            let _ = DeleteObject(icon_info.hbmColor.into());
        }
        if !icon_info.hbmMask.is_invalid() {
            let _ = DeleteObject(icon_info.hbmMask.into());
        }
    }
}

pub fn start_capture() -> Result<bool, Box<dyn Error>> {
    if CAPTURE_RUNNING.swap(true, Ordering::SeqCst) {
        log::error("Capture thread is already running");
        return Err("Capture thread is already running".into());
    }

    let state = match FRAME_STATE.get() {
        Some(state) => state,
        None => {
            return Err("Failed to get frame state: not initialized".into());
        }
    };

    std::thread::spawn(move || -> Result<(), Box<dyn Error + Send + Sync>> {
        let width = state.lock().unwrap().width;
        let height = state.lock().unwrap().height;

        let mut buffer = vec![0u8; width * height * 3];

        while CAPTURE_RUNNING.load(Ordering::Acquire) {
            if let Err(e) = capture_screen_gdi(width, height, &mut buffer) {
                log::error(&format!("Failed to capture screen: {}", e).as_str());
                break;
            }
            if let Ok(mut guard) = state.lock() {
                guard.frame_buffer.copy_from_slice(&buffer);
            }
            FIRST_FRAME_READY.store(true, Ordering::Release);
            // 16ms 是 60fps 的帧率
            std::thread::sleep(std::time::Duration::from_millis(16));
        }
        CAPTURE_RUNNING.store(false, Ordering::Release);

        Ok(())
    });

    Ok(true)
}

pub fn get_frame_buffer() -> Result<&'static [u8], Box<dyn Error>> {
    if !FIRST_FRAME_READY.load(Ordering::Acquire) {
        return Err("Frame not ready yet".into());
    }

    let state = match FRAME_STATE.get() {
        Some(state) => state,
        None => {
            return Err("Failed to get frame state: not initialized".into());
        }
    };

    let mut guard = match state.lock() {
        Ok(guard) => guard,
        Err(_) => {
            return Err("Failed to lock frame state".into());
        }
    };

    // 核心修改：先解引用获取结构体的可变引用
    let state_ref = &mut *guard;
    // 现在 Rust 知道 frame_buffer 和 display_buffer 是不同的字段，允许同时借用
    state_ref
        .display_buffer
        .copy_from_slice(&state_ref.frame_buffer);

    // 将后台缓冲的数据同步到显示缓冲（即 Clone/双缓冲同步）
    // 这一步在锁保护下进行，保证了获取到的画面是完整的（无撕裂）
    let ptr = state_ref.display_buffer.as_ptr();
    let len = state_ref.display_buffer.len();

    // 安全性说明：
    // 1. FRAME_STATE 是 static OnceLock，其内部的 Arc<Mutex<FrameState>> 在初始化后生命周期是 'static 的。
    // 2. guard.frame_buffer 是 Vec<u8>，其在 init 时分配，且在当前实现中从不重新分配（容量固定）。
    // 3. 因此底层指针 ptr 在程序运行期间始终有效。
    // 4. 注意：返回引用后不持有锁，读取时可能与正在写入的 ffmpeg 线程产生数据竞争（画面撕裂），
    //    但在屏幕捕获场景下，这通常比昂贵的内存克隆更可接受。
    Ok(unsafe { std::slice::from_raw_parts(ptr, len) })
}

pub fn stop_capture() -> Result<(), Box<dyn Error>> {
    // let mut ffmpeg_guard = FFMPGE_CMD.lock().unwrap();
    // if let Some(mut child) = ffmpeg_guard.take() {
    //     let _ = child.kill();
    //     let _ = child.wait();
    // }
    // *ffmpeg_guard = None;
    CAPTURE_RUNNING.store(false, Ordering::SeqCst);
    log::info("Capture thread stopped");
    Ok(())
}

pub fn is_frame_ready() -> bool {
    FIRST_FRAME_READY.load(Ordering::Acquire)
}
