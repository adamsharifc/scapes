#ifdef _WIN32

#include "reaper-win.h"
#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <array>
#include <sstream>
#include <iomanip>
#include <limits>

namespace reaper {
const std::string script_filename = "xj3kjd4c.lua";
const WCHAR *REAPER_window_handle_class_name = L"REAPERwnd";

// function prototypes
int send_request_impl(std::string file_path, float start_time, float end_time, float file_duration, int mode);
int write_lua_script(std::string script);
int delete_lua_script(); 
int get_REAPER_pid();
std::string get_file_path_by_pid(int pid);
std::string run_powershell_command(std::string command);
std::string get_REAPER_executable_path();
std::string enclose_quotes(std::string str);
std::string generate_lua_script(std::string file_path, float start_time, float end_time, float file_duration, int mode);
bool is_REAPER_running();                                
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);    // dead code
void print_REAPER_window_handle();                          // dead code

// Public interface function - adapts to internal implementation  
int send_request(float start, float end, float duration, int sampleRate,
				const std::string& fileName, const std::string& filePath) {
	int mode = 0; // insert to selected/active track
	return send_request_impl(filePath, start, end, duration, mode);
}

bool detect() {
	return is_REAPER_running();
}

int send_request_impl(std::string file_path, float start_time, float end_time, float file_duration, int mode){
	// check if reaper is running
	if (!is_REAPER_running()){
		std::cout << "Error: REAPER is not running\n";
		return 1;
	}
	// get the file path of the reaper executable
	std::string reaper_executable_path = get_REAPER_executable_path();
	if (reaper_executable_path.empty()){
		std::cout << "Error: Could not get REAPER executable path\n";
		return 2;
	}
	// generate the lua script
	std::string script = generate_lua_script(file_path, start_time, end_time, file_duration, mode);
	// write the lua script to a file
	if (write_lua_script(script) != 0){
		std::cout << "Error: Could not write lua script\n";
		return 3;
	}
	// run the lua script and capture the output
	std::string cmd_command = enclose_quotes(reaper_executable_path) + " -nonewinst " +  enclose_quotes(script_filename);
	cmd_command = enclose_quotes(cmd_command);
	std::string result = run_powershell_command(cmd_command);
	// delete the lua script after it has been executed
	if (delete_lua_script() != 0){
		std::cout << "Error: Could not delete lua script\n";
		return 4;
	}
	return 0;
}
// Write a lua script to be executed by the reaper DAW
int write_lua_script(std::string script){    
	std::ofstream file(script_filename);
	if (file.is_open()){
		file << script;
		file.close();
	}
	else{
		std::cout << "Error: Could not open file for writing\n";
		return 1;
	}
	return 0;
}
// Delete the lua script after it has been executed
int delete_lua_script(){
	return std::remove(script_filename.c_str()) != 0;
}
// get the process ID of the reaper DAW
int get_REAPER_pid(){
	HWND hwnd = FindWindowW(REAPER_window_handle_class_name, NULL);
	if (hwnd == NULL){
		std::cout << "Error: Could not find REAPER window\n";
		return -1;
	}
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	return pid;
}
// get the file path of the reaper executable
std::string get_file_path_by_pid(int pid){
	std::string command = "powershell.exe Get-Process -Id " + std::to_string(pid) + " -FileVersionInfo ^| Select-Object -ExpandProperty FileName";
	std::string result = run_powershell_command(command);
	if (result.empty()){
		return "";
	}
	// remove newline character from the end of the string
	while (result.back() == '\n' || result.back() == '\r'){
		result.pop_back();
	}
	return result;
}
// utility function for running powershell commands and capturing the output as a string
std::string run_powershell_command(std::string command){
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	if (result.back() == '\n' || result.back() == '\r'){
		result.pop_back();
	}
	return result;
}
// unused function for listing all currently running processes - useful for debugging
// BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
//     char title[80];
//     char className[80];
//     GetWindowText(hwnd, title, sizeof(title));
//     GetClassName(hwnd, className, sizeof(className));

//     std::ofstream file("windows.txt", std::ios::app);
//     std::string titleStr(title);
//     // cout << "Window handle: " << hwnd << ", title: " << title << ", class name: " << className << std::endl;
//     if (file.is_open()){
//         file << "Window handle: " << hwnd << ", title: " << title << ", class name: " << className << std::endl;
//         file.close();
//     }
//     else{
//         std::cout << "Error: Could not open file for writing\n";
//         return 1;
//     }
//     // std::cout << "Window handle: " << hwnd << ", title: " << title << ", class name: " << className << std::endl;
//     return TRUE;
// }
// unused print the window handle of the reaper DAW - useful for debugging
void print_REAPER_window_handle(){
	HWND hwnd = FindWindowW(REAPER_window_handle_class_name, NULL);
	if (hwnd == NULL){
		std::cout << "Error: Could not find REAPER window\n";
		return;
	}    
	std::cout << "REAPER window handle: " << hwnd << std::endl;
}
// check if the reaper DAW is running
bool is_REAPER_running(){
	return FindWindowW(REAPER_window_handle_class_name, NULL) != NULL;
}
// get the file path of the reaper executable
std::string get_REAPER_executable_path(){
	int pid = get_REAPER_pid();
	if (pid == -1){
		return "";
	}
	return get_file_path_by_pid(pid);
}
// enclose a string in quotes
std::string enclose_quotes(std::string str){
	return "\"" + str + "\"";
}
// get the lua script to insert a media section into the reaper DAW
std::string generate_lua_script(std::string file_path, float start_time, float end_time, float file_duration, int mode){
	std::ostringstream script;
	script << std::setprecision(std::numeric_limits<float>::max_digits10)
		   << "local file_path = [[" << file_path << "]]\n"
		   << "local start_time = " << start_time << "\n"
		   << "local end_time = " << end_time << "\n"
		   << "local file_duration = " << file_duration << "\n"
		   << "local mode = " << mode << "\n"
		   << "local start_percent = start_time / file_duration\n"
		   << "local end_percent = end_time / file_duration\n"
		   << "reaper.InsertMediaSection(file_path, mode, start_percent, end_percent, 0)\n";
	return script.str();
}
} // namespace reaper

#endif // _WIN32