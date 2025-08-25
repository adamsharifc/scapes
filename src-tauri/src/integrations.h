#pragma once

#include <string>


namespace protools {
	int send_request(float start, float end, float duration, int sampleRate, 
					const std::string& fileName, const std::string& filePath);
	bool detect();
}

namespace reaper {
	int send_request(float start, float end, float duration, int sampleRate,
					const std::string& fileName, const std::string& filePath);
	bool detect();
}

namespace detector {
	bool is_DAW_running(const std::string& daw);
	std::string Scan_All();
}

extern "C" {
	int _SendToTimeline_(float start, float end, float duration, int sampleRate, 
						const char* fileName, const char* filePath, const char* daw);
	bool _is_DAW_running_(const char* daw);
	const char* _Scan_All_Daws_();
}
