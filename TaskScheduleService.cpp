#include "TaskScheduleService.h"
#include "RcConfig.h"
#include <ctime>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <fstream>
#include "GenericImageService.h"
#include "global.h"
#include "utils.h"
#include <loader.h>
#include "IPlugin.h"
#include <tuple>    
#include <list>

TaskScheduleService::TaskScheduleService() {
	taskRepository.reset(new TaskRepository);
};

std::shared_ptr<TaskScheduleService> TaskScheduleService::dpt{new (std::nothrow) TaskScheduleService()};

std::shared_ptr<TaskScheduleService>& TaskScheduleService::GetInstance()
{
	return dpt;
}

std::string _initializedSubTask(Task t, std::string moduleName,std::string step) {
	ProcessPattern pp;
	char sql[2048];
	snprintf(sql, 2048, "select UUID from processpattern SATID=%d and PAYLOADID=%d and WAVEBANDID=%d and INTEGRALTIME=%f and MODULE=%s;",
		t.satId, t.payloadId, t.waveBandId, t.integralTime, moduleName);
	//DeNoiseModule
	sql::ResultSet* result = g_stmt->executeQuery(sql);
	SubTask st;

	strcpy(st.taskId, t.uuid);
	strcpy(st.processPatternId, result->getString("UUID").c_str());
	strcpy(st.step, step.c_str());
	st.status = 0;
	g_stmt->execute(st.insert());

	delete result;
	sql::ResultSet* r = g_stmt->executeQuery("select last_insert_id() as id");
	std::string taskId = r->getString("id");
	delete r;
	return taskId;
};

void TaskScheduleService::initializeTask()
{
	char sql[2048];
	snprintf(sql, 2048, "select SATID,PAYLOADID,WAVEBANDID,INTEGRALTIME from genericimage where LIFESTATUS=0 group by SATID,PAYLOADID,WAVEBANDID,INTEGRALTIME;");

	sql::ResultSet* result = g_stmt->executeQuery(sql);

	while (result->next()) {
		int satId = result->getInt("SATID");
		int payloadId = result->getInt("PAYLOADID");
		int waveBandId = result->getInt("WAVEBANDID");
		double IT = result->getDouble("INTEGRALTIME");

		Task t;
		t.cycleId = 0;
		t.satId = satId;
		t.waveBandId = waveBandId;
		t.integralTime = IT;
		snprintf(sql, 2048, "select min(STARTTIMESTAMP) as STARTTIMESTAMP, max(ENDTIMESTAMP) as ENDTIMESTAMP  from genericimage \
						where SATID=%d and PAYLOADID=%d and WAVEBANDID=%d and INTEGRALTIME=%f;",
			satId,payloadId,waveBandId,IT);

		sql::ResultSet* timeResult = g_stmt->executeQuery(sql);
		strcpy(t.startTimeStamp, timeResult->getString("STARTTIMESTAMP").c_str());
		strcpy(t.endTimeStamp, timeResult->getString("ENDTIMESTAMP").c_str());
		t.status = 0; //pending

		g_stmt->execute(t.insert());
		
		sql::ResultSet* r= g_stmt->executeQuery("select last_insert_id() as id");
		std::string taskId = r->getString("id");
		strcpy(t.uuid, taskId.c_str());
		delete r;

		snprintf(sql, 2048, "UPDATE genericimage SET taskId = %d WHERE satId = %d and  payloadId = %d and  waveBandId = %d and  integralTime = %f and taskId=0;",
			taskId, t.satId, t.payloadId, t.waveBandId, t.integralTime);
		g_stmt->execute(sql);

		std::string step1=_initializedSubTask(t, "UNCModule", "");
		std::string step2 = _initializedSubTask(t, "BLDModule", step1);
		std::string step3 = _initializedSubTask(t, "AutoBLDModule", step2);
		std::string step4 = _initializedSubTask(t, "BackgroundModule", step3);
		std::string step5 = _initializedSubTask(t, "MergePixelModule", step4);
		std::string step6 = _initializedSubTask(t, "MultiFrameModule", step5);
		std::string step7 = _initializedSubTask(t, "DeNoiseModule", step6);

		t.status = 1;
		g_stmt->execute(t.update());

		//update image status to extracted
		snprintf(sql, 2048, "UPDATE genericimage SET lifeStatus = %d WHERE satId = %d and  payloadId = %d and  waveBandId = %d and  integralTime = %f;",
			1, t.satId, t.payloadId, t.waveBandId, t.integralTime);
		g_stmt->execute(sql);
	}

	delete result;
}



std::future<bool> TaskScheduleService::preloading()
{
	return  std::async([&]() {
		/*
		* todo
		* 1. change task status, find all relative image
		* 2. load parameters as context
		* 3. genericImageService process each image,  whether image is ready and change task status
		* 5. ObjectDetectionService process each image, whether label is read and change task status
		* 6. ReportGenerationService process each label/image, whether report is read and change task status
		* 7. check task status, whether exception occurs log it
		* 8. update task status, failed/successed
		*/

		jws::RcConfig rcconfig;
		bool ret = rcconfig.ReadConfig("cfg.ini");
		if (ret == false) {
			printf("ReadConfig is Error,Cfg=%s", "cfg.ini");
			return false;
		}

		std::string remoteDatRootPath = rcconfig.ReadString("DAT", "REMOTEROOTPATH", "C:/Users/86138/Desktop/zdg_calibratio/FSDB_BC_JWS_20230706/FSDB_BC_JWS_20230706/data");

		//std::string satCode;
		//if (t.satId = 5122)
		//{
		//	satCode = "HY1402";
		//}
		//else if (t.satId = 5123)
		//{
		//	satCode = "HY1403";
		//}
		//else
		//{
		//	return;
		//}
		////todo
		//std::string waveBandCode;
		//if (t.satId = 5122)
		//{
		//	if (t.waveBandId == 1) {
		//		waveBandCode = "DBHY";
		//	};
		//	if (t.waveBandId == 2) {
		//		waveBandCode = "ZBHY";
		//	};
		//	if (t.waveBandId == 3) {
		//		waveBandCode = "ZCHY";
		//	};
		//	if (t.waveBandId == 4) {
		//		waveBandCode = "CBHY";
		//	};
		//	if (t.waveBandId > 10) {
		//		waveBandCode = "HDPY";
		//	};
		//}
		//else if (t.satId = 5123)
		//{
		//	if (t.waveBandId == 1) {
		//		waveBandCode = "YDRY";
		//	};
		//	if (t.waveBandId == 2) {
		//		waveBandCode = "ZBHY";
		//	};
		//	if (t.waveBandId == 3) {
		//		waveBandCode = "ZCHY";
		//	};
		//	if (t.waveBandId == 4) {
		//		waveBandCode = "CBHY";
		//	};
		//	if (t.waveBandId > 10) {
		//		waveBandCode = "HDRY";
		//	};
		//}
		//else
		//{
		//	return;
		//}

		std::vector<std::string> satCodes;
		std::vector<std::string> waveBandCodes;
		
		// 获取当前时间戳
		time_t now = time(nullptr);

		// 将时间戳转换为 `tm` 结构
		tm* local_time = localtime(&now);

		// 格式化日期
		char buffer[10];
		strftime(buffer, sizeof(buffer), "%Y%m%d", local_time);
		std::string ddate(buffer);

		if (g_datcache.size() > 7) {
			g_datcache.clear();
		}
		if (!g_datcache.count("ddate")) {
			std::vector<std::string> datCache;
			g_datcache.insert(std::make_pair("ddate", datCache));
		}


		std::vector<std::string> batchDats;
		//step1： 在各星，各谱段，在当日文件夹中检查最新dat文件
		for (auto& satCode : satCodes) {
			for (auto& waveBandCode : waveBandCodes) {
				remoteDatRootPath = remoteDatRootPath + '/' + satCode + '/' + waveBandCode + '/' + ddate;

				
				std::experimental::filesystem::path dir = remoteDatRootPath;
				for (const auto& entry : std::experimental::filesystem::directory_iterator(dir)) {
					std::string firstLayer = remoteDatRootPath + '/' + satCode + '/' + waveBandCode + '/' + ddate + '/' + entry.path().string();
					for (const auto& subentry : std::experimental::filesystem::directory_iterator(firstLayer)) {
						std::string secondLayer = remoteDatRootPath + '/' + satCode + '/' + waveBandCode + '/' + ddate
							+ '/' + entry.path().string() + '/' + subentry.path().string();
						for (const auto& datName : std::experimental::filesystem::directory_iterator(secondLayer)) {
							if (std::find(g_datcache["ddate"].begin(), g_datcache["ddate"].end(), datName) == g_datcache["ddate"].end()) {

								std::string datPath = remoteDatRootPath + '/' + satCode + '/' + waveBandCode + '/' + ddate
									+ '/' + entry.path().string() + '/' + subentry.path().string() + '/' + datName.path().string();

								batchDats.push_back(datPath);
								g_datcache["ddate"].push_back(datName.path().string());
							}
						}
					}
				}
			}
		}
		

		

		auto imageService=GenericImageService::GetInstance();

		//step2: copy 数据到本地,eg 50份dat， 50*2k*2K*2/1024/1024/12=400s,50*30/1000=1.5s
		for (std::string& p : batchDats) {
			
			//step3: 入库
			imageService->loadImage(p, IMAGELEVEL::ONE);
		}

		initializeTask();
		return true;
		});
}

bool _checkValid(std::map<std::string, std::tuple<BaseImagePluginInterface*, std::map<std::string, double>, std::map<std::string, std::string>>>& algoDict) {
	if (!algoDict.count("UNCModule")) {
		std::cerr << "[TaskScheduleService] not subtask module" << "UNCModule" << std::endl;
		return false;
	}
	if (!algoDict.count("BLDModule")) {
		std::cerr << "[TaskScheduleService] not subtask module" << "BLDModule" << std::endl;
		return false;
	}
	if (!algoDict.count("BackgroundModule")) {
		std::cerr << "[TaskScheduleService] not subtask module" << "BackgroundModule" << std::endl;
		return false;
	}
	if (!algoDict.count("AutoBLDgroundModule")) {
		std::cerr << "[TaskScheduleService] not subtask module" << "AutoBLDgroundModule" << std::endl;
		return false;
	}
	if (!algoDict.count("MergePixelModule")) {
		std::cerr << "[TaskScheduleService] not subtask module" << "MergePixelModule" << std::endl;
		return false;
	}
	if (!algoDict.count("MultiFrameModule")) {
		std::cerr << "[TaskScheduleService] not subtask module" << "MultiFrameModule" << std::endl;
		return false;
	}
	if (!algoDict.count("DeNoiseModule")) {
		std::cerr << "[TaskScheduleService] not subtask module" << "DeNoiseModule" << std::endl;
		return false;
	}
	return true;
};

std::future<bool> TaskScheduleService::executeTask()
{
	/*
	* 3. exeute each task, change status
	* 4. 
	*/
	return std::async([&]() {
		char sql[2048];
		snprintf(sql, 2048, "select * from task where LIFESTATUS!=1 limit 1;");
		std::string taskId;
		sql::ResultSet* result = g_stmt->executeQuery(sql);
		while (result->next()) {
			taskId = result->getString("UUID");
		}

		delete result;
		if (taskId.empty()) return true;

		snprintf(sql, 2048, "select UUID, LOCALFILEPATH, STARTFRAMECOUNT from genericimage where taskId=%d order by STARTFRAMECOUNT",taskId);
		result = g_stmt->executeQuery(sql);

		std::map<std::string,std::string> dats;
		while (result->next()) {
			dats.insert({ result->getString("UUID"),result->getString("LOCALFILEPATH") });
		}
		delete result;

		std::map<std::string, std::tuple<BaseImagePluginInterface*, std::map<std::string, double> , std::map<std::string, std::string>>> algoDict;
		snprintf(sql, 2048, "select * from SUBTASK where taskId=%d left join processpattern on  SUBTASK.PROCESSPATTERNID=processpattern.UUID", taskId);
		result = g_stmt->executeQuery(sql);

		PluginManager ma;
		while (result->next()) {
			std::string module = result->getString("MODULE");
			std::string algorithm = result->getString("ALGORITHM");
			std::map<std::string,double> rawParams = UTILS::strToMapD(result->getString("PARAMETERS"));
			std::map<std::string,std::string> rawCondition = UTILS::strToMapS(result->getString("CONDITIONS"));
			IPluginFactory* infoA = ma.addPlugin(module);
			BaseImagePluginInterface* algoInstance = (BaseImagePluginInterface*)infoA->Factory(algorithm.c_str());
			std::tuple<BaseImagePluginInterface*, std::map<std::string, double>, std::map<std::string, std::string>> entry(algoInstance, rawParams, rawCondition);

			algoDict.insert({ "MODULE" ,entry });
			delete infoA;
		}
		delete result;
		if(!_checkValid(algoDict)) return false;
		
		std::list<cv::Mat> imgCache;

		auto imageService = GenericImageService::GetInstance();
		for (auto dat = dats.begin();dat != dats.end();dat++) {

		}

		return true;
		});

}

void TaskScheduleService::stopTask(uint32_t cycleId)
{
}
