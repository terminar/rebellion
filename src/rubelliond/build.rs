//use cc::Build;
//use std::path::Path;
//extern crate gcc;

fn main() {
    /*
    let project_root = Path::new(env!("CARGO_MANIFEST_DIR"));
    let native_src = project_root.join("native");

    Build::new()
        .flag_if_supported("-lcurl")
        .file(native_src.join("simple.c"))
        .file(native_src.join("better.c"))
        .file(native_src.join("curl.c"))
        .compile("native");

    println!("cargo:rustc-link-lib=curl");
    */

//    println!("cargo:rustc-link-search=native=/home/path/to/rust/proyect/folder/contain/file.a");
//    println!("cargo:rustc-link-lib=static=test");
//    println!("cargo:rustc-link-search=native=../../build/lib/"); // +
    println!("cargo:rustc-link-search=native=../../"); // +
    println!("cargo:rustc-link-lib=dylib=rebellion");

/*
use bindgen::builder;

    // Configure and generate bindings.
    let bindings = builder().header("../../include/rebellion.h")
        .allowlist_type("rebellion.*")
        .allowlist_var("rebellion.*")
        .allowlist_function("REBELLION.*")
        .generate()?;

    // Write the generated bindings to an output file.
    bindings.write_to_file("src/rebellion-bindings.rs")?;
*/
}
