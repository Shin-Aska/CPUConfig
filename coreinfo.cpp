#include "coreinfo.h"

CoreInfo::CoreInfo(int pid, std::string vendor, int cpuFamilyId, int modelId, std::string modelName)
{
    this->processorId = pid;
    this->vendorId = vendor;
    this->cpuFamilyId = cpuFamilyId;
    this->modelId = modelId;
    this->modelName = modelName;
}

CoreInfo::CoreInfo()
{

}
