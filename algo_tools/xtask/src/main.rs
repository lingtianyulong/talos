use std::{
    fs,
    io::{self, Write},
    path::PathBuf,
    process::{exit, Command, Stdio},
};

fn algo_tools_root() -> PathBuf {
    let xtask_dir = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    xtask_dir
        .parent()
        .unwrap_or_else(|| std::path::Path::new("."))
        .to_path_buf()
}

fn target_dir() -> PathBuf {
    algo_tools_root().join("target")
}

// 复制文件到目标目录，只复制 .dll 和 .lib 文件
#[allow(unused)]
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
                fs::rename(&path, &new_path)
                    .expect(&format!("rename file {} failed.", path.display()));
                println!(
                    "rename file {} to {} success.",
                    path.display(),
                    new_path.display()
                );
            }
        }
    }
    Ok(())
}

fn main() {
    let root = algo_tools_root();
    println!("the root of the project is: {}", root.display());

    let mut target_dir = target_dir();
    println!("the target directory is: {}", target_dir.display());

    let is_release = std::env::args().any(|arg| arg == "--release");
    if is_release {
        target_dir = target_dir.join("release");
        println!("the release target directory is: {}", target_dir.display());
    } else {
        target_dir = target_dir.join("debug");
        println!("the debug target directory is: {}", target_dir.display());
    }

    let projects = ["base_tool", "color_convert_tool"];

    for project in projects {
        println!("building project: {}", project);
        io::stdout().flush().unwrap();

        let mut cmd = Command::new("cargo");
        cmd.arg("build");
        if is_release {
            cmd.arg("--release");
        }
        cmd.arg("-p").arg(project);

        // 显式继承标准输出和错误流，确保能看到 cargo 的构建过程
        cmd.stdout(Stdio::inherit());
        cmd.stderr(Stdio::inherit());

        let status = cmd.status().expect("failed to execute cargo build.");
        if !status.success() {
            eprintln!("build project {} failed: {}", project, status);
            exit(status.code().unwrap_or(1));
        }
        println!("build {} success.", project);

        // 修正：cargo 产物默认直接在 target/debug 或 target/release 下，不带项目名子目录
        let src_dir = target_dir.clone();
        remame_lib_name(&src_dir).expect("rename lib name failed.");
        println!("rename lib name in {} success.", src_dir.display());
    }
}
