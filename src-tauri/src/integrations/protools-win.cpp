#ifdef _WIN32

#include "protools-win.h"
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

namespace protools{
// function prototypes
int send_request_impl(std::string filePath, short trackOffset, long sampleLoc, short stream, long regionStart_p, long regionStop_p, std::string regionName);
int spotter(std::string xmlString);
std::string getXMLString(std::string filePath, short trackOffset, long sampleLoc, short stream, long regionStart_p, long regionStop_p, std::string regionName);

// Public interface function - adapts to internal implementation
int send_request(float start, float end, float duration, int sampleRate,
				const std::string& fileName, const std::string& filePath) {
	short trackOffset = 0;
	long sampleLoc = 0;
	short stream = 1;	// 1 for left ... etc
	long regionStart_p = start * sampleRate;
	long regionStop_p = end * sampleRate;
	std::string regionName = "scapes_" + fileName;
	return send_request_impl(filePath, trackOffset, sampleLoc, stream, regionStart_p, regionStop_p, regionName);
}

bool detect() {
	return FindWindowW(L"ptOpenWindowClass", NULL) != NULL;
}

int send_request_impl(std::string filePath, short trackOffset, long sampleLoc, short stream, long regionStart_p, long regionStop_p, std::string regionName){
	
	std::string xml = getXMLString(filePath, trackOffset, sampleLoc, stream, regionStart_p, regionStop_p, regionName);
	std::cout << xml << std::endl;
	int result = spotter(xml);
	if (result == 0){
		std::cout << "Success\n";
	} else if (result == -1){
		std::cout << "Window not found\n";
	} else if (result == -2){
		std::cout << "Failed to send message\n";
	}
	return result;
}
int spotter(std::string xmlString){
	// spotter cannot be called directly from the integration.cpp file
	// because of errors in the following line;
	// argument of type "const char *" is incompatible with parameter of type "LPCWSTR" (aka "const WCHAR *")C/C++(167)
	HWND hwnd = ::FindWindowW(L"ptOpenWindowClass", L"ptOpenWindow");
	if (hwnd)
	{
		COPYDATASTRUCT cds;
		cds.dwData = 'XMLR';
		cds.lpData = (void*)xmlString.c_str();
		cds.cbData = (DWORD)xmlString.size() + 1;

		if(!(::SendMessage(hwnd, WM_COPYDATA, NULL, (LPARAM)&cds)))
			return -2;
	}
	else return -1;
	return 0;
}
std::string getXMLString(std::string filePath, short trackOffset, long sampleLoc, short stream, long regionStart_p, long regionStop_p, std::string regionName){

	short trackNum = -99;
	short frameFormat = 0;

	std::ostringstream os;
																				// Equivalent AppleEvent keywords
	os  << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"           << std::endl
		<< "<request>"                                            << std::endl
		<< "    <action>spotRegion</action>"                      << std::endl
		<< "    <file>"        << filePath  << "</file>"          << std::endl  // FILE
		<< "    <trackNum>"    << trackNum    << "</trackNum>"    << std::endl  // Trak
		<< "    <frameFormat>" << frameFormat << "</frameFormat>" << std::endl  // FFrm
		<< "    <trackOffset>" << trackOffset << "</trackOffset>" << std::endl  // TkOf
		<< "    <sampleLoc>"   << sampleLoc   << "</sampleLoc>"   << std::endl  // SMSt
		<< "    <stream>"      << stream      << "</stream>"      << std::endl  // Strm
		<< "    <region>"                                         << std::endl  // Rgn
		<< "        <start>" << regionStart_p << "</start>"       << std::endl  // Star
		<< "        <stop>"  << regionStop_p  << "</stop>"        << std::endl  // Stop
		<< "        <name>"  << regionName  << "</name>"          << std::endl  // Name
		<< "    </region>"                                        << std::endl
		<< "</request>"                                           << std::endl
		<< std::ends;

	return os.str();
}
} // namespace protools

#endif // _WIN32