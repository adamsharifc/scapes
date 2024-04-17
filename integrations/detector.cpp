#include <stdio.h>
#include <windows.h>
#include <cstdlib>
#include <iostream>

const WCHAR *REAPER = L"REAPERwnd";
const WCHAR *PROTOOLS = L"ptOpenWindowClass";

bool is_DAW_running(const WCHAR *window_handle_class_name);

int main(){
    printf("Hello, World!\n");
    
    std::cout << std::boolalpha << is_DAW_running(REAPER) << std::endl;
    std::cout << std::boolalpha << is_DAW_running(PROTOOLS) << std::endl;
    return 0;
}


bool is_DAW_running(const WCHAR *window_handle_class_name){
    return FindWindowW(window_handle_class_name, NULL) != NULL;
}