const CC_SOURCES: &str = "src/cc";

fn main() {
    let mut build = cc::Build::new();

    build
        .cpp(true)
        .files(&[
            format!("{}/emu65x64.cpp", CC_SOURCES),
            format!("{}/mem65x64.cpp", CC_SOURCES),
            format!("{}/nozo65x64.cpp", CC_SOURCES),
        ]);

    if cfg!(debug_assertions) {
        build
            .warnings(true)
            .extra_warnings(true);
    } else {
        build.flag("-O3");
    }

    build.compile("emu65x64");

    println!("cargo:rerun-if-changed={}/emu65x64.cpp", CC_SOURCES);
    println!("cargo:rerun-if-changed={}/emu65x64.hpp", CC_SOURCES);
    println!("cargo:rerun-if-changed={}/mem65x64.cpp", CC_SOURCES);
    println!("cargo:rerun-if-changed={}/mem65x64.hpp", CC_SOURCES);
    println!("cargo:rerun-if-changed={}/nozo65x64.cpp", CC_SOURCES);
    println!("cargo:rerun-if-changed={}/nozo65x64.hpp", CC_SOURCES);
}
