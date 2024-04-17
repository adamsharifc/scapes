// src/integrations.cpp
#include <stdio.h>

extern "C" {
  	bool UniqueFunction(int a, int b) {
    	return false;
  	}
	int _SendToTimeline_(float start, float end, float duration, int sampleRate, const char* fileName, const char* filePath, const char* daw) {
		
		const char* localfilename = "localfilename";
		printf("Hello from C++\n");
		printf("Start: %f\n", start);
		printf("End: %f\n", end);
		printf("Duration: %f\n", duration);
		printf("SampleRate: %d\n", sampleRate);
		printf("FileName: %s\n", fileName);
		printf("FilePath: %s\n", filePath);
		printf("DAW: %s\n", daw);
		return 10;
	}
}