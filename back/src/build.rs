// src/build.rs
fn main() {
    
    let src = [
        "src/integrations.cpp",
        "src/integrations/protools-win.cpp",
        "src/integrations/reaper-win.cpp",
        "src/integrations/detector.cpp"
    ];
        
    cc::Build::new()
     .cpp(true)
     .files(src)
     .compile("integrations.a");
}

// force rebuild