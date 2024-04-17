#include <string>  
#include <stdio.h>
#include <windows.h>
#include <cstdlib>

const WCHAR *PROTOOLS = L"ptOpenWindowClass";
const WCHAR *REAPER = L"REAPERwnd";

namespace detector{

    bool detect_window(const WCHAR *window_handle_class_name);
    bool is_DAW_running(const WCHAR *window_handle_class_name);
    std::string Scan_All();

    bool detect_window(const WCHAR *window_handle_class_name){
        return FindWindowW(window_handle_class_name, NULL) != NULL;
    }
    bool is_DAW_running(std::string daw){
        if (daw == "protools-win"){return detect_window(PROTOOLS);}
        else if (daw == "reaper-win"){return detect_window(REAPER);}
        return false;
    }
    std::string Scan_All(){
        std::string daws;
        if (detect_window(PROTOOLS)){
            daws += "protools-win,";
        }
        if (detect_window(REAPER)){
            daws += "reaper-win,";
        }
        
        if (!daws.empty()){
            daws.pop_back(); // remove last comma
        }
        return daws;
    }

} // namespace detector