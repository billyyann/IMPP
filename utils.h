#include "common_define.h"
#include <string>
#include <map>
#include <vector>

#pragma once

namespace UTILS {
	namespace TIME {
		TIMESTAMP cur_dt();
	};
	namespace STR {

		std::vector<std::string> split(const std::string& str, char delimiter);
	}
	
	std::map < std::string, double > strToMapD(std::string);
	std::map < std::string, std::string > strToMapS(std::string);

	std::string mapToStr(std::map<std::string, int32_t>);
}