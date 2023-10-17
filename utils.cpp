#include "utils.h"
#include <sstream>

namespace UTILS {
    namespace TIME {
        TIMESTAMP cur_dt()
        {
            struct tm* pt;
            time_t t1;
            t1 = time(NULL);
            pt = localtime(&t1);

            TIMESTAMP tim;
            tim._year = pt->tm_year + 1900;
            tim._mon = pt->tm_mon + 1;
            tim._day = pt->tm_mday;
            tim._hour = pt->tm_hour;
            tim._min = pt->tm_min;
            tim._sec = pt->tm_sec;
            tim._ms = 0.0;
            return tim;

        }
    }

    std::map < std::string, std::string > strToMapS(std::string str) {
        std::stringstream ss(str);

        // Create a map.
        std::map<std::string, std::string> res;

        // Iterate over the string stream and split the key-value pairs.
        std::string key, value;
        while (std::getline(ss, key, ':')) {
            std::getline(ss, value);
            res[key] = value;
        }

        return res;
    };

    std::map < std::string, double > strToMapD(std::string str)
    {
        std::stringstream ss(str);

        // Create a map.
        std::map<std::string, double> res;

        // Iterate over the string stream and split the key-value pairs.
        std::string key, value;
        while (std::getline(ss, key, ':')) {
            std::getline(ss, value);
            res[key] = atof(value.c_str());
        }

        return res;
    };

    std::string mapToStr(std::map<std::string, int32_t> mp)
    {
        std::stringstream ss;

        // Iterate over the map and concatenate the key-value pairs into the string stream.
        for (const auto& pair : mp) {
            ss << pair.first << ":" << pair.second << ", ";
        }
        return ss.str();
    };
}

std::vector<std::string> UTILS::STR::split(const std::string& str, char delimiter)
{
    std::vector<std::string> res;
    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++) {

        // If we reached the end of the word or the end of the input.
        if (str[i] == delimiter || i == str.size()) {
            endIndex = i;
            std::string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            res.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
    return res;
}
