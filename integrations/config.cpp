#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
using std::cout;
using std::endl;

int main(){
    
    std::unordered_map<std::string, std::string> myMap;

    myMap["key1"] = "value1";
    myMap["key2"] = "value2";
    
    cout << "Hello World!\n";

    cout << myMap["key1"] << endl;
    cout << myMap["key2"] << endl;


    return 0;

}  