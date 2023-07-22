#include "cpuinfo.h"

std::string CPUInfo::systemQuery(std::string query) {
    std::ifstream cpu_info_stream;
    std::string result;
    std::string line;
    cpu_info_stream.open(query);
    if (cpu_info_stream.is_open()) {
        while (std::getline(cpu_info_stream, line)) {
            if (result.length() == 0) {
                result += line;
            }
            else {
                result += "\n" + line;
            }
        }
    }
    return result;
}

bool CPUInfo::updateFromSystemQuery(std::string query, std::string value){
    std::ofstream cpu_info_stream;
    cpu_info_stream.open(query);
    if (cpu_info_stream.is_open()) {
        cpu_info_stream << value;
        cpu_info_stream.close();
        return true;
    }
    else {
        return false;
    }
}

std::string CPUInfo::formatify(CPUSpeed chosenConversionSpeed, int current_speed) {
    std::string output;
    std::stringstream sstream;
    if (chosenConversionSpeed == CPUSpeed::hz) {
        sstream << current_speed << " hz";
    }
    else if (chosenConversionSpeed == CPUSpeed::mhz) {
        sstream << current_speed / CPUSpeed::mhz << " mhz";;
    }
    else if (chosenConversionSpeed == CPUSpeed::ghz) {
        sstream << current_speed / CPUSpeed::ghz << " ghz";;
    }
    output = sstream.str();
    return output;
}

int CPUInfo::getCurrentCPUFrequency(void) {
    std::string frequency = CPUInfo::systemQuery("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    return Utility::convertStringToInt((frequency));
}

std::string CPUInfo::getCurrentCPUFrequency(CPUSpeed chosenConversionSpeed) {
    int frequency = CPUInfo::getCurrentCPUFrequency();
    return CPUInfo::formatify(chosenConversionSpeed, frequency);
}

int CPUInfo::getMinimumAllowableFrequency() {
    std::string frequency = CPUInfo::systemQuery("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq");
    return Utility::convertStringToInt((frequency));
}

std::string CPUInfo::getMinimumAllowableFrequency(CPUSpeed chosenConversionSpeed) {
    int frequency = CPUInfo::getMinimumAllowableFrequency();
    return CPUInfo::formatify(chosenConversionSpeed, frequency);
}

int CPUInfo::getMaximumAllowableFrequency() {
    std::string frequency = CPUInfo::systemQuery("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    return Utility::convertStringToInt((frequency));
}
std::string CPUInfo::getMaximumAllowableFrequency(CPUSpeed chosenConversionSpeed) {
    int frequency = CPUInfo::getMaximumAllowableFrequency();
    return CPUInfo::formatify(chosenConversionSpeed, frequency);
}



int CPUInfo::getMinimumCurrentFrequency() {
    std::string frequency = CPUInfo::systemQuery("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq");
    return Utility::convertStringToInt((frequency));
}
std::string CPUInfo::getMinimumCurrentFrequency(CPUSpeed chosenConversionSpeed) {
    int frequency = CPUInfo::getMinimumCurrentFrequency();
    return CPUInfo::formatify(chosenConversionSpeed, frequency);
}

int CPUInfo::getMaximumCurrentFrequency() {
    std::string frequency = CPUInfo::systemQuery("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq");
    return Utility::convertStringToInt((frequency));
}

std::string CPUInfo::getMaximumCurrentFrequency(CPUSpeed chosenConversionSpeed) {
    int frequency = CPUInfo::getMaximumCurrentFrequency();
    return CPUInfo::formatify(chosenConversionSpeed, frequency);
}

std::string CPUInfo::getProcessorInfoString() {
    return CPUInfo::systemQuery("/proc/cpuinfo");
}

bool CPUInfo::setMinimumFrequency(int cpuId, int value, CPUSpeed speed) {
    std::string cpu_id = "cpu" + Utility::convertIntToString(cpuId);
    std::string target_value = Utility::convertIntToString(value * speed);
    return CPUInfo::updateFromSystemQuery("/sys/devices/system/cpu/" + cpu_id + "/cpufreq/scaling_min_freq", target_value);
}

bool CPUInfo::setMaximumFrequency(int cpuId, int value, CPUSpeed speed) {
    std::string cpu_id = "cpu" + Utility::convertIntToString(cpuId);
    std::string target_value = Utility::convertIntToString(value * speed);
    return CPUInfo::updateFromSystemQuery("/sys/devices/system/cpu/" + cpu_id + "/cpufreq/scaling_max_freq", target_value);
}

std::vector<CoreInfo> CPUInfo::getProcessorInfo(){
    std::vector<CoreInfo> result;
    std::string cpuInfo = CPUInfo::getProcessorInfoString();
    std::vector<std::string> information = Utility::splitString(cpuInfo, '\n');

    CoreInfo cur_info;
    for (size_t i = 0; i < information.size(); i++) {
        std::string line = information.at(i);
        if (line.length() > 0) {
            std::vector<std::string> info = Utility::splitString(line, '\t');
            if (info.size() >= 2) {
                std::string key = info.at(0);
                std::string value;
                if (info.size() == 2) {
                    value = info.at(1).substr(2);
                }
                else if (info.size() == 3) {
                    value = info.at(2).substr(2);
                }

                if (key == "processor") {
                    cur_info.processorId = Utility::convertStringToInt(value);
                }
                else if (key == "vendor_id") {
                    cur_info.vendorId = value;
                }
                else if (key == "core id") {
                    cur_info.coreId = Utility::convertStringToInt(value);
                }
                else if (key == "cpu family") {
                    cur_info.cpuFamilyId = Utility::convertStringToInt(value);
                }
                else if (key == "model") {
                    cur_info.modelId = Utility::convertStringToInt(value);
                }
                else if (key == "model name") {
                    cur_info.modelName = value;
                }
                //qDebug() << QString::fromLocal8Bit((key + "\t: " + value).c_str()) ;
            }
        }
        else {
            result.push_back(cur_info);
        }
    }
    result.push_back(cur_info);

    sensors_init(nullptr);
    sensors_chip_name const * cn;
    int chip_int_pointer_reference = 0;

    while ((cn = sensors_get_detected_chips(0, &chip_int_pointer_reference)) != 0) {
        qDebug() << "Chip: " << cn->prefix << "/" << cn->path << " => " << cn->addr << " -> [" << cn->bus.nr << "," << cn->bus.type << "]";
        std::string chip_name(cn->prefix);
        if (chip_name.find("coretemp") != std::string::npos) {

            sensors_feature const *feat;
            int feature_int_pointer_reference = 0;

            while ((feat = sensors_get_features(cn, &feature_int_pointer_reference)) != 0) {
                std::string label(sensors_get_label(cn, feat));
                std::vector<std::string> label_breakdown = Utility::splitString(label, ' ');
                int coreId = Utility::convertStringToInt(label_breakdown.at(1));
                qDebug() << label.c_str() << "(" << feat->name << ")" << feat->number << " " << feat->padding1;

                sensors_subfeature const *subf;
                int subfeature_int_pointer_reference = 0;

                while ((subf = sensors_get_all_subfeatures(cn, feat, &subfeature_int_pointer_reference)) != 0) {
                    std::string featureName(subf->name);
                    double val;

                    if (subf->flags & SENSORS_MODE_R) {
                        int rc = sensors_get_value(cn, subf->number, &val);
                        if (rc < 0) {
                            qDebug() << "An error occured while trying to fetch value for " << subf->name << "err: " << rc;
                        } else {
                            if (featureName.find("input") != std::string::npos && label_breakdown.size() == 2 && label_breakdown.at(0) == "Core") {
                                for (size_t i = 0; i < result.size(); i++) {
                                    if (result.at(i).coreId == coreId) {
                                        result.at(i).temperature = val;
                                    }
                                }
                            }
                            qDebug() << subf->name << "=" << val;
                        }
                    }
                }
            }
        }
    }
    return result;
}
