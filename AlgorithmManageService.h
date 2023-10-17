#pragma once
#include <string>
#include <memory>
#include "repository.h"

class AlgorithmManageService
{
public:
	static std::shared_ptr<AlgorithmManageService>& GetInstance();
	
	bool addPluginAlgorithm(std::string module, std::string algorithm, std::map<std::string, int32_t>& params, std::map<std::string, int32_t>& condition);
private:
	AlgorithmManageService();

	static std::shared_ptr<AlgorithmManageService> dpt;
	std::shared_ptr < ProcessPatternRepository> processPatternRepository{ nullptr };
};

