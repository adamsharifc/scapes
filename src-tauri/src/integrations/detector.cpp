#include "detector.h"
#include <string>  
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#include <cstdlib>

const WCHAR *PROTOOLS = L"ptOpenWindowClass";
const WCHAR *REAPER = L"REAPERwnd";
#endif

namespace detector {

#ifdef _WIN32
	bool detect_window(const WCHAR *window_handle_class_name);
	
	bool detect_window(const WCHAR *window_handle_class_name) {
		return FindWindowW(window_handle_class_name, NULL) != NULL;
	}
#endif

	bool is_DAW_running(const std::string& daw) {
#ifdef _WIN32
		if (daw == "protools-win" || daw == "protools") {
			return detect_window(PROTOOLS);
		}
		else if (daw == "reaper-win" || daw == "reaper") {
			return detect_window(REAPER);
		}

#elif defined(__APPLE__)
		// Add macOS DAW detection here
		if (daw == "protools-mac" || daw == "protools") {
			// TODO: Implement macOS ProTools detection
			return false;
		}

#elif defined(__linux__)
		// Add Linux DAW detection here
		if (daw == "reaper-linux" || daw == "reaper") {
			// TODO: Implement Linux Reaper detection
			return false;
		}
#endif

		return false;
	}
	std::string Scan_All() {
		std::string daws = "";
		bool is_anything_running = false;
		
#ifdef _WIN32        
		if (detect_window(PROTOOLS)) {
			daws.append("protools-win,");   // strange bug += not working append working
			is_anything_running = true;
		}
		
		if (detect_window(REAPER)) {
			daws.append("reaper-win,");
			is_anything_running = true;
		}

#elif defined(__APPLE__)
		// Add macOS DAW scanning here
		// if (detect_protools_mac()) { 
		//     daws.append("protools-mac,"); 
		//     is_anything_running = true; 
		// }

#elif defined(__linux__)
		// Add Linux DAW scanning here  
		// if (detect_reaper_linux()) { 
		//     daws.append("reaper-linux,"); 
		//     is_anything_running = true; 
		// }
#endif
		
		if (is_anything_running == false) {
			return "none";
		}
		
		return daws;
	}

} // namespace detector