#include "utility.h"
int Utility::convertStringToInt(std::string number) {
    int result = -1;
    std::stringstream sstream(number);
    sstream >> result;
    return result;
}

std::string Utility::convertIntToString(int number) {
    std::string result;
    std::stringstream sstream;
    sstream << number;
    sstream >> result;
    return result;
}

std::vector<std::string> Utility::splitString(std::string text, char ch) {
    std::vector<std::string> result;
    std::istringstream cpuInfoBuffer(text);
    std::string line;
    while (std::getline(cpuInfoBuffer, line, ch)) {
        result.push_back(line);
    }
    return result;
}
