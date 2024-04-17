#![allow(non_snake_case)]

use std::ffi::{c_float, CString};
use std::os::raw::c_char;

extern "C" {
    pub fn _SendToTimeline_(start: c_float, end: c_float, duration: c_float, sampleRate: i32, fileName: *const c_char, filePath: *const c_char, daw: *const c_char)->i32;
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
