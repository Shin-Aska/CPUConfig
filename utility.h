#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


class Utility
{
public:
    static int convertStringToInt(std::string);
    static double convertStringToDouble(std::string);
    static std::string convertIntToString(int);
    static std::string convertDoubleToString(double);
    static std::vector<std::string> splitString(std::string, char);
};

#endif // UTILITY_H
