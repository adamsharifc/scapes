#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

int rotator();
int sendRequest_(std::string filePath, short trackOffset, long sampleLoc, short stream, long regionStart_p, long regionStop_p, std::string regionName);
int spotter(std::string xmlString);
std::string getXMLString(std::string filePath, short trackOffset, long sampleLoc, short stream, long regionStart_p, long regionStop_p, std::string regionName);

extern "C"{
    int sendRequest(
        const char* filePath, 
        short trackOffset, 
        long sampleLoc, 
        short stream, 
        long regionStart_p, 
        long regionStop_p, 
        const char* regionName
    ){
        std::string filePath_ = std::string(filePath);
        std::string regionName_ = std::string(regionName);
        return sendRequest_(filePath_, trackOffset, sampleLoc, stream, regionStart_p, regionStop_p, regionName_);
    }
}
int sendRequest_(std::string filePath, short trackOffset, long sampleLoc, short stream, long regionStart_p, long regionStop_p, std::string regionName){
    
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
    HWND hwnd = ::FindWindowW("ptOpenWindowClass", "ptOpenWindow");
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