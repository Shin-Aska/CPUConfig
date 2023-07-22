#ifndef CPUINFO_H
#define CPUINFO_H


#include <iostream>
#include <fstream>
#include <sstream>

#include <QDebug>
#include <sensors.h>

#include "coreinfo.h"
#include "utility.h"

enum CPUSpeed {
    hz = 1, mhz = 1000, ghz = 1000000
};

class CPUInfo
{
    private:
        static std::string systemQuery(std::string);
        static bool updateFromSystemQuery(std::string, std::string);
        static std::string formatify(CPUSpeed, int);
        static std::string getProcessorInfoString();
    public:
        static int getCurrentCPUFrequency(void);
        static std::string getCurrentCPUFrequency(CPUSpeed);

        static int getMinimumAllowableFrequency(void);
        static std::string getMinimumAllowableFrequency(CPUSpeed);

        static int getMaximumAllowableFrequency(void);
        static std::string getMaximumAllowableFrequency(CPUSpeed);

        static int getMinimumCurrentFrequency(void);
        static std::string getMinimumCurrentFrequency(CPUSpeed);

        static int getMaximumCurrentFrequency(void);
        static std::string getMaximumCurrentFrequency(CPUSpeed);

        static std::vector<CoreInfo> getProcessorInfo(void);
        static bool setMinimumFrequency(int, int, CPUSpeed);
        static bool setMaximumFrequency(int, int, CPUSpeed);
};

#endif // CPUINFO_H
