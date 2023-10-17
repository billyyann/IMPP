#pragma once
#include <cstdint>
#include "repository.h"
#include <memory>
#include <future>
#include <thread>


class TaskScheduleService
{
public:
	static std::shared_ptr<TaskScheduleService>& GetInstance();
	std::future<bool>  executeTask();
	void stopTask(uint32_t cycleId);

private:
	TaskScheduleService();
	//每30分钟扫一次硬盘，
	std::future<bool> preloading();
	void initializeTask();

	static std::shared_ptr<TaskScheduleService> dpt;
	std::shared_ptr < TaskRepository> taskRepository{ nullptr };
	std::vector<std::string > datCache;
};

