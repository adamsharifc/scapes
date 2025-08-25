#pragma once

#include <string>

namespace protools {
	int send_request(float start, float end, float duration, int sampleRate,
					const std::string& fileName, const std::string& filePath);
	bool detect();
}
