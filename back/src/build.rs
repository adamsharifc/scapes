// src/build.rs
fn main() {
    cc::Build::new()
     .cpp(true)
     .file("src/integrations.cpp")
     .compile("integrations.a");
}

// force