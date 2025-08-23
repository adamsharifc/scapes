// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

fn main() {
    scapes_lib::run()
}

// // Prevents additional console window on Windows in release, DO NOT REMOVE!!
// #![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

// mod metadata_extract;
// mod integrations;

// fn main() {
//   tauri::Builder::default()
//     .invoke_handler(tauri::generate_handler![
//       my_custom_command,
//       scan_all_daws
//     ])
//     .run(tauri::generate_context!())
//     .expect("error while running tauri application");
// }

// #[tauri::command]
// fn my_custom_command() -> String {
//   println!("Hello from Rust!");
//   metadata_extract::main();
//   return "Hello from Rust!".into()
// }

// #[tauri::command]
// fn scan_all_daws() -> String{
//   let result = integrations::scan_all_daws();
//   return result;
// }