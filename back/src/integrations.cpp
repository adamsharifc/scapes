// src/integrations.cpp
#include <stdio.h>
#include "integrations/protools-win.h"
#include "integrations/reaper-win.cpp"


int SendToTimeline(float start, float end, float duration, int sampleRate, const char* c_fileName, const char* c_filePath, const char* c_daw);
int handleProtoolsWin(float start, float end, float duration, int sampleRate, std::string fileName, std::string filePath);
int handleReaperWin(float start, float end, float duration, int sampleRate, std::string fileName, std::string filePath);
bool DetectDaw(std::string daw);

extern "C" {
	int _SendToTimeline_(float start, float end, float duration, int sampleRate, const char* fileName, const char* filePath, const char* daw) {
		
		SendToTimeline(start, end, duration, sampleRate, fileName, filePath, daw);
		return 10;
	}
	bool _DetectDaw_(const char* daw){
		std::string dawStr = std::string(daw);
		return DetectDaw(dawStr);
	}
}

int SendToTimeline(float start, float end, float duration, int sampleRate, const char* c_fileName, const char* c_filePath, const char* c_daw) {

	// convert to const char* to std::strings
	std::string fileName = std::string(c_fileName);
	std::string filePath = std::string(c_filePath);
	std::string daw = std::string(c_daw);

	if (daw == "protools-win"){
		handleProtoolsWin(start, end, duration, sampleRate, fileName, filePath);
	}
	else if (daw == "reaper-win"){
		handleReaperWin(start, end, duration, sampleRate, fileName, filePath);
	}
	else{
		printf("DAW not supported\n");
	}
	return 0;
}

int handleProtoolsWin(float start, float end, float duration, int sampleRate, std::string fileName, std::string filePath) {
	short trackOffset = 0;
	long sampleLoc = 0;
	short stream = 1;	// 1 for left ... etc
	long regionStart_p = start * sampleRate;
	long regionStop_p = end * sampleRate;
	std::string regionName = "scapes_" + fileName;
	int result = sendRequest_(filePath, trackOffset, sampleLoc, stream, regionStart_p, regionStop_p, regionName);
	return 0;
}
int handleReaperWin(float start, float end, float duration, int sampleRate, std::string fileName, std::string filePath) {
	printf("handling Reaper\n");
	int mode = 0; // insert to selected/active track
	int result = reaper_win::send_request(filePath, start, end, duration, mode);
	return 0;
}
bool DetectDaw(std::string daw){
	printf("Detecting DAW\n");
	return true;
}	