#![allow(non_snake_case)]

use std::ffi::{c_float, CString};
use std::os::raw::c_char;

extern "C" {
    fn _SendToTimeline_(start: c_float, end: c_float, duration: c_float, sampleRate: i32, fileName: *const c_char, filePath: *const c_char, daw: *const c_char)->i32;
    fn _is_DAW_running_(daw: *const c_char)->bool;
    fn _Scan_All_Daws_()-> *const c_char;
}

pub fn SendToTimeline(start: f32, end: f32, duration: f32, sampleRate: i32, fileName: &str, filePath: &str, daw: &str)->i32 {
    unsafe {
        let fileName = CString::new(fileName).unwrap();
        let filePath = CString::new(filePath).unwrap();
        let daw = CString::new(daw).unwrap();
        let result = _SendToTimeline_(start, end, duration, sampleRate, fileName.as_ptr(), filePath.as_ptr(), daw.as_ptr());
        return result;
    }
}
pub fn is_DAW_running(daw: &str)->bool {
    unsafe {
        let daw = CString::new(daw).unwrap();
        let result = _is_DAW_running_(daw.as_ptr());
        return result;
    }
}
pub fn Scan_All_Daws()-> String {
    unsafe {
        let result = _Scan_All_Daws_();
        let c_str = std::ffi::CStr::from_ptr(result);
        return c_str.to_str().unwrap().to_string();
    }
}
pub fn scan_all_daws()->String{
    let daws = vec!["protools-win", "reaper-win"];
    let mut anything_running = false;
    let mut result = String::new();
    for daw in daws{
        if is_DAW_running(daw){
            result.push_str(daw);
            result.push_str(",");
            anything_running = true;
        }
    }
    if anything_running{    // remove the last comma
        result.pop();
    }
    else{
        result.push_str("none");
    }
    return result;
}