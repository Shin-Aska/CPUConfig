#ifndef COREINFO_H
#define COREINFO_H

#include <iostream>

class CoreInfo
{
public:
    CoreInfo(int, std::string, int, int, std::string);
    CoreInfo();
    int processorId;
    std::string vendorId;
    int cpuFamilyId;
    int modelId;
    std::string modelName;
};

#endif // COREINFO_H
