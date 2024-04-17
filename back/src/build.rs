// src/build.rs
fn main() {
    
    let src = [
        "src/integrations.cpp",
        "src/integrations/protools-win.cpp",
    ];
        
    cc::Build::new()
     .cpp(true)
     .files(src)
     .compile("integrations.a");
}

// force rebuild