// src/integrations.cpp
#include <iostream>
#include <string>
#include "integrations.h"
#include "integrations/detector.h"

#ifdef _WIN32
#include "integrations/protools-win.h"
#include "integrations/reaper-win.h"
#endif

// Simple implementation functions
int SendToTimeline(float start, float end, float duration, int sampleRate, 
                  const std::string& fileName, const std::string& filePath, 
                  const std::string& daw);

extern "C" {
	int _SendToTimeline_(float start, float end, float duration, int sampleRate, 
		const char* fileName, const char* filePath, 
		const char* daw) {
		
			return SendToTimeline(start, end, duration, sampleRate, 
			std::string(fileName), std::string(filePath), std::string(daw));
	}
	bool _is_DAW_running_(const char* daw){
		std::string dawStr = std::string(daw);
		return detector::is_DAW_running(dawStr);
	}
	const char* _Scan_All_Daws_(){
		static std::string result = detector::Scan_All();
		return result.c_str();
	}
}

int SendToTimeline(float start, float end, float duration, int sampleRate, 
                  const std::string& fileName, const std::string& filePath, 
                  const std::string& daw) {

#ifdef _WIN32
	if (daw == "protools-win" || daw == "protools") {
		return protools::send_request(start, end, duration, sampleRate, fileName, filePath);
	}
	else if (daw == "reaper-win" || daw == "reaper") {
		return reaper::send_request(start, end, duration, sampleRate, fileName, filePath);
	}

#elif defined(__APPLE__)
	if (daw == "protools-mac" || daw == "protools") {
		// TODO: Implement macOS ProTools integration
		std::cerr << "ProTools integration not implemented for macOS yet" << std::endl;
		return -2; // Not implemented
	}
	else if (daw == "reaper-mac" || daw == "reaper") {
		// TODO: Implement macOS Reaper integration
		std::cerr << "Reaper integration not implemented for macOS yet" << std::endl;
		return -2; // Not implemented
	}

#elif defined(__linux__)
	if (daw == "reaper-linux" || daw == "reaper") {
		// TODO: Implement Linux Reaper integration
		std::cerr << "Reaper integration not implemented for Linux yet" << std::endl;
		return -2; // Not implemented
	}
	// Note: ProTools doesn't run on Linux

#endif
	
	std::cerr << "DAW not supported on this platform: " << daw << std::endl;
	return -1; // Unsupported DAW/platform combination
}