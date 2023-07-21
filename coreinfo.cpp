#include "coreinfo.h"

CoreInfo::CoreInfo(int pid, int cid, std::string vendor, int cpuFamilyId, int modelId, std::string modelName, double coreTemperature)
{
    this->processorId = pid;
    this->coreId = cid;
    this->vendorId = vendor;
    this->cpuFamilyId = cpuFamilyId;
    this->modelId = modelId;
    this->modelName = modelName;
    this->temperature = coreTemperature;
}

CoreInfo::CoreInfo()
{

}
