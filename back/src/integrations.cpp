// src/integrations.cpp
#include <stdio.h>
#include "integrations/protools-win.h"

int matchDaw(float start, float end, float duration, int sampleRate, const char* c_fileName, const char* c_filePath, const char* c_daw);
int handleProtoolsWin(float start, float end, float duration, int sampleRate, std::string fileName, std::string filePath);

extern "C" {
	int _SendToTimeline_(float start, float end, float duration, int sampleRate, const char* fileName, const char* filePath, const char* daw) {
		
		printf("Start: %f\n", start);
		printf("End: %f\n", end);
		printf("Duration: %f\n", duration);
		printf("SampleRate: %d\n", sampleRate);
		printf("FileName: %s\n", fileName);
		printf("FilePath: %s\n", filePath);
		printf("DAW: %s\n", daw);
		matchDaw(start, end, duration, sampleRate, fileName, filePath, daw);
		return 10;
	}
}

int matchDaw(float start, float end, float duration, int sampleRate, const char* c_fileName, const char* c_filePath, const char* c_daw) {

	// convert to const char* to std::strings
	std::string fileName = std::string(c_fileName);
	std::string filePath = std::string(c_filePath);
	std::string daw = std::string(c_daw);

	if (daw == "protools-win"){
		printf("Protools Windows\n");
		handleProtoolsWin(start, end, duration, sampleRate, fileName, filePath);
	}
	else if (daw == "reaper-win"){
		printf("Reaper windows\n");
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