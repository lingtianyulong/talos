use std::error::Error;
use std::ffi::{CStr, CString};
use std::time::Duration;
use std::os::raw::c_char;
use reqwest::blocking::Client;
use reqwest::header::{HeaderValue, CONTENT_TYPE, USER_AGENT, AUTHORIZATION};

// 发送 POST 请求, 参数为 url 和 json 字符串
#[unsafe(no_mangle)]
pub extern "C" fn http_post(url: *const c_char, body: *const c_char, auth: *const c_char) -> *mut c_char {
    println!("In http post");

    if url.is_null() {
        return std::ptr::null_mut();
    }

    let url = unsafe { CStr::from_ptr(url) };
    let body = unsafe { CStr::from_ptr(body) };
    let auth = unsafe { CStr::from_ptr(auth) };

    let url_str = match url.to_str() {
        Ok(s) => s,
        Err(_) => return std::ptr::null_mut(),
    };

    // 安全转换 json body
    let json_body = match body.to_str() {
        Ok(body) => body,
        Err(_) => return std::ptr::null_mut(),
    };

    let auth_str = match auth.to_str() {
        Ok(s) => s,
        Err(_) => return std::ptr::null_mut(),
    };

    println!("url_str: {}", url_str);
    println!("json_body: {}", json_body);

    let mut headers = reqwest::header::HeaderMap::new();
    headers.insert(CONTENT_TYPE, HeaderValue::from_static("application/json"));
    headers.insert(USER_AGENT, HeaderValue::from_static("curl/7.88.0"));

    if !auth_str.is_empty() && auth_str.len() != 0 {
        let auth_value = format!("Bearer {}", auth_str);
        // from_str 生成拥有的 HeaderValue，避免临时字符串生命周期问题
        match HeaderValue::from_str(&auth_value) {
            Ok(v) => {
                headers.insert(AUTHORIZATION, v);
            }
            Err(e) => {
                eprintln!("Invalid auth header: {}", e);
                return std::ptr::null_mut();
            }
        }
    }


    let client = Client::builder()
        .no_proxy()
        .default_headers(headers)
        .timeout(Duration::from_secs(20))
        .build().unwrap();

    let res = client
        .post(url_str)
        .header("Content-Type", "application/json")
        .body(json_body.to_string())
        .send();

    match res {
        Ok(resp) => {
            let text = resp.text();
            match text {
                Ok(body) => CString::new(body).unwrap().into_raw(),
                Err(e) => {
                    eprintln!("Failed to read body: {}", e);
                    std::ptr::null_mut()
                }
            }
        }
        Err(e) => {
            // 打印详细错误链
            eprintln!("POST failed: {}", e);
            let mut source = e.source();
            while let Some(inner) = source {
                eprintln!("Caused by: {}", inner);
                source = inner.source();
            }
            std::ptr::null_mut()
        }
    }
}

// 释放返回的字符串
#[unsafe(no_mangle)]
pub extern "C" fn free_http_response(s: *mut c_char) {
    if !s.is_null() {
        return;
    }
    unsafe {
        _ = CString::from_raw(s)
    }
}
