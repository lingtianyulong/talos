use std::{
    fs, io,
    path::PathBuf,
    process::{Command, exit},
};

// 获取 rust_kernel 项目的根目录
fn kernel_root() -> PathBuf {
    let xtask_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    xtask_dir.parent().unwrap().to_path_buf()
}

// rust_kernel 项目的父目录
fn kernel_parent_root() -> PathBuf {
    let kernel_root = kernel_root();
    kernel_root.parent().unwrap().to_path_buf()
}

// 复制文件到目标目录，只复制 .dll 和 .lib 文件
fn copy_to_dst(src_dir: PathBuf, dst_dir: PathBuf) -> io::Result<()> {
    if !src_dir.exists() {
        return Err(io::Error::new(io::ErrorKind::NotFound, "源目录不存在"));
    }

    if !dst_dir.exists() {
        fs::create_dir_all(&dst_dir).expect(&format!("创建目录 {} 失败", dst_dir.display()));
    }

    for entry in fs::read_dir(src_dir)? {
        let entry = entry?;
        let path = entry.path();
        if path.is_file() {
            if let Some(extension) = path.extension().and_then(|e| e.to_str()) {
                if extension == "dll" || extension == "lib" {
                    let file_name = path.file_name().unwrap().to_string_lossy();
                    let dst_path = dst_dir.join(file_name.as_ref());
                    fs::copy(&path, &dst_path).expect(&format!("复制文件 {} 失败", path.display()));
                    println!("复制文件 {} 成功", path.display());
                }
            }
        }
    }
    Ok(())
}

// 遍历目录下的所有文件，如果文件名以 .dll.lib 结尾，则重命名为 .lib
fn remame_lib_name(dir: &PathBuf) -> io::Result<()> {
    for entry in fs::read_dir(dir)? {
        let entry = entry?;
        let path = entry.path();
        if let Some(name) = path.file_name().and_then(|n| n.to_str()) {
            if name.ends_with(".dll.lib") {
                let new_name = name.replace(".dll.lib", ".lib");
                let new_path = dir.join(new_name);
                fs::rename(&path, &new_path).expect(&format!("重命名文件 {} 失败", path.display()));
                println!("重命名文件 {} 成功", path.display());
            }
        }
    }
    Ok(())
}

fn main() {
    let root = kernel_parent_root();
    println!("项目根目录: {}", root.display());

    let mut dst_dir = root.join("rust_libs");
    // 判断是否是 release 模式
    let is_release = std::env::args().any(|arg| arg == "--release");
    if is_release {
        dst_dir = dst_dir.join("release");
        println!("release 目录: {}", dst_dir.display());
    } else {
        dst_dir = dst_dir.join("debug");
        println!("debug 目录: {}", dst_dir.display());
    }

    if !dst_dir.exists() {
        fs::create_dir_all(&dst_dir).expect(&format!("创建目录 {} 失败", dst_dir.display()));
        return;
    }

    let projects = [
        "logger",
        "encryption",
        "database",
        "libdump",
        "secure_store",
        "netlib",
        "screen_capture",
    ];
    for project in projects {
        println!("正在构建项目: {}", project);
        let mut cmd = Command::new("cargo");
        cmd.arg("build");
        if is_release {
            cmd.arg("--release");
        }
        cmd.arg("-p").arg(project);
        let status = cmd.status().expect("构建失败");
        if !status.success() {
            eprintln!("构建失败: {}", status);
            exit(status.code().unwrap_or(1));
        }
        println!("{} 构建成功", project);

        let src_dir =
            kernel_root()
                .join("target")
                .join(if is_release { "release" } else { "debug" });

        remame_lib_name(&src_dir).expect("重命名文件失败");

        println!("源目录: {}", src_dir.display());
        println!("复制文件到目标目录: {}", dst_dir.display());
        copy_to_dst(src_dir, dst_dir.clone()).expect("复制失败");
    }
}
