#ifndef COREINFO_H
#define COREINFO_H

#include <iostream>

class CoreInfo
{
public:
    CoreInfo(int, int, std::string, int, int, std::string, double);
    CoreInfo();
    int processorId;
    int coreId;
    std::string vendorId;
    int cpuFamilyId;
    int modelId;
    std::string modelName;
    double temperature;
};

#endif // COREINFO_H
