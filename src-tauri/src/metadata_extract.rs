#![allow(dead_code)]

use std::collections::HashMap;
use serde_json::Value;
use chrono;

pub fn main(){
	println!("Metadata Extracted!");
	let dir_path = "C:\\Users\\adams\\Music\\bbcsounds";
	scan_dir(dir_path);
}
pub fn exec_command(command: &str) -> String {
	let output = std::process::Command::new("powershell")
		.arg("-c")
		.arg(command)
		.output()
		.expect("failed to execute process");

	let output = String::from_utf8(output.stdout).unwrap();
	return output;
}
pub fn scan_dir(path: &str){

	let relative_path_to_exe = "../../utils/exif.exe";
	let output = exec_command(format!("{} -r {}", relative_path_to_exe ,path).as_str());
	let delimiter = "========";
	let mut lines: Vec<&str> = output.split(delimiter).collect();

	for i in 0..lines.len(){
		let j = extract_json_from_line(lines[i]);
		println!("{}", serde_json::to_string_pretty(&j).unwrap());
	}
	// let j = extract_json_from_line(lines[lines.len()-1]);
	// println!("{}", serde_json::to_string_pretty(&j).unwrap());

}
fn extract_json_from_line(line: &str)->Value{
	
	let mut hm: HashMap<String, Value> = HashMap::new();
	let lines: Vec<&str> = line.split("\n").collect();
	let file_path = lines[0].trim();
	hm.insert("File Path".to_string(), Value::String(file_path.to_string()));
	
	let mut k: &str;
	let mut v: &str;
	let mut v_value: Value;
	// skip first two lines
	// line 0 is the file path
	// line 1 is the exif tool version
	for index in 2..lines.len(){
		if lines[index].starts_with(" ") || lines[index].is_empty(){
			continue;
		}
		let kv: Vec<&str> = lines[index].splitn(2,":").collect();
		k = kv[0].trim();
		v = kv[1].trim();
		v_value = parse_value(v);
		hm.insert(k.to_string(), v_value);
	}
	return serde_json::from_value(serde_json::to_value(hm).unwrap()).unwrap();
}
fn parse_value(s: &str) -> Value {
	if let Ok(i) = s.parse::<i64>() {
		return Value::Number(i.into());
	} else if let Ok(b) = s.parse::<bool>() {
		return Value::Bool(b);
	} else if let Ok(dt) = chrono::NaiveDateTime::parse_from_str(s, "%Y-%m-%d %H:%M:%S"){
		return Value::String(dt.to_string());
	}
	return Value::String(s.to_string())
}