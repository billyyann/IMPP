#include "AlgorithmManageService.h"



AlgorithmManageService::AlgorithmManageService() {
	processPatternRepository.reset(new ProcessPatternRepository);
};

std::shared_ptr<AlgorithmManageService> AlgorithmManageService::dpt{new (std::nothrow) AlgorithmManageService()};

std::shared_ptr<AlgorithmManageService>& AlgorithmManageService::GetInstance()
{
	return dpt;
}

bool AlgorithmManageService::addPluginAlgorithm(std::string module, std::string algorithm,std::map<std::string, int32_t>& params,std::map<std::string, int32_t>& condition)
{
	ProcessPattern p;
	if (params.count("SATID")) {
		p.satId = params["SATID"];
	}
	if (params.count("PAYLOADID")) {
		p.payloadId = params["PAYLOADID"];
	}
	if (params.count("WAVEBANDID")) {
		p.waveBandId = params["WAVEBANDID"];
	}
	if (params.count("INTEGRALTIME")) {
		p.integralTime = params["INTEGRALTIME"];
	}

	strcpy(p.parameters, UTILS::mapToStr(params).c_str());
	strcpy(p.condition, UTILS::mapToStr(condition).c_str());
	strcpy(p.module ,module.c_str());
	strcpy(p.algorithm ,algorithm.c_str());
	processPatternRepository->addOne(p);

    return true;
}
