fn main() {
	tauri_build::build();

	// detect the target OS set by cargo
	let target_os = std::env::var("CARGO_CFG_TARGET_OS").unwrap_or_default();

	// common sources
	let mut sources = vec![
		"src/integrations.cpp",
		"src/integrations/detector.cpp",
	];

	// add platform-specific files only for their target OS
	if target_os == "windows" {
		sources.push("src/integrations/protools-win.cpp");
		sources.push("src/integrations/reaper-win.cpp");
	} else if target_os == "macos" {
		// Future macOS implementation
		// sources.push("src/integrations/protools-mac.mm");
		// sources.push("src/integrations/reaper-mac.cpp");
	} else if target_os == "linux" {
		// Future Linux implementation
		// sources.push("src/integrations/reaper-linux.cpp");
	}

	let mut build = cc::Build::new();
	build.cpp(true);
	build.include("src"); // Add include path for headers
	
	for src in sources {
		build.file(src);
		println!("cargo:rerun-if-changed={}", src);
	}
	
	// Also watch header files for changes
	if target_os == "windows" {
		println!("cargo:rerun-if-changed=src/integrations/protools-win.h");
		println!("cargo:rerun-if-changed=src/integrations/reaper-win.h");
	}
	println!("cargo:rerun-if-changed=src/integrations/detector.h");
	println!("cargo:rerun-if-changed=src/integrations.h");
	
	build.compile("integrations.a");

	// force rebuild please
	println!("cargo:rerun-if-changed=build.rs");
}