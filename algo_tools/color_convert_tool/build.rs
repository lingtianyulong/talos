use std::env;
use std::path::PathBuf;

fn main() {
    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());

    // Cargo sets PROFILE to "debug" or "release"
    let profile = env::var("PROFILE").unwrap();

    // The rust_libs directory is at the root of the project
    // algo_tools/color_convert_tool -> parent -> parent -> rust_libs
    let mut lib_dir = manifest_dir
        .parent()
        .expect("Failed to get parent of manifest dir")
        .parent()
        .expect("Failed to get project root")
        .join("rust_libs");

    if profile == "release" {
        lib_dir = lib_dir.join("release");
    } else {
        lib_dir = lib_dir.join("debug");
    }

    println!("cargo:rustc-link-search=native={}", lib_dir.display());
    println!("cargo:rerun-if-changed=build.rs");
}
