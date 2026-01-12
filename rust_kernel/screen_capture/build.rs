use std::env;
use std::path::PathBuf;

fn main() {
    // 获取输出目录
    let out_dir = env::var("OUT_DIR").unwrap();
    let target_dir = PathBuf::from(&out_dir)
        .ancestors()
        .nth(3)
        .unwrap()
        .to_path_buf();

    // 告诉 cargo 在哪里找 logger.dll 的导入库
    println!("cargo:rustc-link-search=native={}", target_dir.display());

    // 当 logger 重新编译时，重新编译 screen_capture
    println!("cargo:rerun-if-changed={}/logger.dll", target_dir.display());
}
