#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

int sendRequest_(std::string filePath, short trackOffset, long sampleLoc, short stream, long regionStart_p, long regionStop_p, std::string regionName);
int spotter(std::string xmlString);
std::string getXMLString(std::string filePath, short trackOffset, long sampleLoc, short stream, long regionStart_p, long regionStop_p, std::string regionName);