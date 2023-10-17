#pragma once
#include <string>
#include "dao.h"
#include <vector>
#include "global.h"
#include <mysql/jdbc.h>
#include "utils.h"


typedef struct _TaskHeader {
	uint32_t cycleId;
	uint32_t satId;
	uint32_t payloadId;
	uint32_t waveBandId;
} TaskHeader;


class GenericImageRepository
{
public:
	GenericImageRepository() {
		//connect(dbconn);
	};
	~GenericImageRepository() {
		//dbconn.close();
	};

	bool addAll(std::vector<GenericImage>& dats) {
		for (GenericImage& img : dats) {
			addOne(img);
		}
		return true;
	};
	bool addOne(GenericImage& dat)
	{
		
		std::string sql(dat.insert());
		std::cout << sql << std::endl;
		g_stmt.get()->executeQuery(sql);
		return true;
	};
	//bool deleteById();

	bool updateById(GenericImage& dat, int taskId)
	{	
		//update lifestatus
		//todo
		g_stmt.get()->executeQuery(dat.update());
		return true;
	};

	std::vector<GenericImage> searchAllById(int cycleId) {
		std::vector<GenericImage> res;
		char sql[2048];
		snprintf(sql, 2048, "select * from genericimage where cycleId=%d ;", cycleId);

		sql::ResultSet* result=g_stmt.get()->executeQuery(sql);
		
		while (result->next()) {
			GenericImage tmp;
			tmp.processStationId = result->getInt("processStationId");
			tmp.taskId = result->getInt("taskId");
			tmp.satId = result->getInt("satId");
			tmp.payloadId = result->getInt("payloadId");
			tmp.waveBandId = result->getInt("waveBandId");
			tmp.channelId = result->getInt("channelId");

			tmp.integralTime = result->getDouble("integralTime");
			strcpy(tmp.startTimeStamp,result->getString("startTimeStamp").c_str());
			strcpy(tmp.endTimeStamp, result->getString("endTimeStamp").c_str());

			tmp.startFrameCount = result->getInt("startFrameCount");
			tmp.endFrameCount = result->getInt("endFrameCount");
			tmp.stareWorkMode = result->getInt("stareWorkMode");

			strcpy(tmp.gentime, UTILS::TIME::cur_dt().toString().c_str());

			tmp.subFileLen = result->getInt("subFileLen");
			tmp.subFileNum = result->getInt("subFileNum");

			strcpy(tmp.remoteFilePath, result->getString("remoteFilePath").c_str());
			strcpy(tmp.localFilePath, result->getString("localFilePath").c_str());
			
			tmp.lifeStatus = result->getInt("lifeStatus");
			strcpy(tmp.entryTime , result->getString("entryTime").c_str());

			tmp.payloadStatus = result->getInt("payloadStatus");
			
			res.push_back(tmp);
		}
		delete result;
		return res;
	};
private:
	//QSqlDatabase dbconn;
};


class TaskRepository
{
public:
	TaskRepository() {
		//connect(dbconn);
	};
	~TaskRepository() { 
		//dbconn.close();
	};

	bool addAll(std::vector< Task>& dats) {
		for (Task& img : dats) {
			addOne(img);
		}
		return true;
	};
	bool addOne(Task& dat)
	{
		std::string sql(dat.insert());
		g_stmt->execute(sql);
		return true;
	};
	//bool deleteById();

	bool updateById(Task& dat, int cycleId)
	{
		//update lifestatus
		g_stmt.get()->executeQuery(dat.update());
		return true;
	};

	std::vector< Task> searchAllById(int cycleId) {
		std::vector< Task> res;
		char sql[2048];
		snprintf(sql, 2048, "select * from task where cycleId=%d ;", cycleId);

		sql::ResultSet* result = g_stmt.get()->executeQuery(sql);

		while (result->next()) {
			Task tmp;
			tmp.cycleId = result->getInt("cycleId");
			tmp.satId = result->getInt("satId");
			strcpy(tmp.startTimeStamp , result->getString("startTimeStamp").c_str());
			strcpy(tmp.endTimeStamp ,result->getString("endTimeStamp").c_str());
			strcpy(tmp.entryTime , result->getString("entryTime").c_str());
			tmp.status = result->getInt("status");

			res.push_back(tmp);
		}
		delete result;
		return res;
	};
private:
	//QSqlDatabase dbconn;
};


class ProcessPatternRepository
{
public:
	ProcessPatternRepository() {
		//connect(dbconn);
	};
	~ProcessPatternRepository() {
		//dbconn.close();
	};

	bool addAll(std::vector< ProcessPattern>& dats) {
		for (ProcessPattern& img : dats) {
			addOne(img);
		}
		return true;
	};
	bool addOne(ProcessPattern& dat)
	{
		g_stmt.get()->executeQuery(dat.insert());
		
		return true;
	};
	//bool deleteById();

	bool updateById(ProcessPattern& dat, int cycleId)
	{
		//update lifestatus
		g_stmt.get()->executeQuery(dat.update());
		return true;
	};

	std::vector< ProcessPattern> searchAllById(std::string moduleName) {
		std::vector< ProcessPattern> res;
		char sql[2048];
		snprintf(sql, 2048, "select * from processpattern where module=%d ;", moduleName);

		sql::ResultSet* result = g_stmt.get()->executeQuery(sql);

		while (result->next()) {
			ProcessPattern tmp;
			tmp.satId = result->getInt("satId");
			tmp.payloadId = result->getInt("payloadId");
			tmp.waveBandId = result->getInt("waveBandId");
			tmp.integralTime = result->getDouble("integralTime");
			strcpy(tmp.module ,result->getString("module").c_str());
			strcpy(tmp.algorithm, result->getString("algorithm").c_str());
			strcpy(tmp.parameters, result->getString("parameters").c_str());
			strcpy(tmp.condition, result->getString("condition").c_str());
			strcpy(tmp.entryTime,result->getString("entryTime").c_str());
			tmp.status = result->getInt("status");

			res.push_back(tmp);
		}
		delete result;
		return res;
	};
private:
	//QSqlDatabase dbconn;
};


class ProcessRecordRepository
{
public:
	ProcessRecordRepository() {
		//connect(dbconn);
	};
	~ProcessRecordRepository() {  };

	bool addAll(std::vector< ProcessRecord>& dats) {
		for (ProcessRecord& img : dats) {
			addOne(img);
		}
		return true;
	};
	bool addOne(ProcessRecord& dat)
	{
		g_stmt.get()->executeQuery(dat.insert());
	
		return true;
	};
	//bool deleteById();

	bool updateById(ProcessRecord& dat, int cycleId)
	{
		//update lifestatus
		g_stmt.get()->executeQuery(dat.update());
		return true;
	};

	std::vector< ProcessRecord> searchAllById(int cycleId) {
		std::vector< ProcessRecord> res;
		char sql[2048];
		snprintf(sql, 2048, "select * from processrecord where cycleId=%d ;", cycleId);

		sql::ResultSet* result = g_stmt.get()->executeQuery(sql);

		while (result->next()) {
			ProcessRecord tmp;
			tmp.genericImageId = result->getInt("genericImageId");
			tmp.taskId = result->getInt("taskId");
			strcpy(tmp.entryTime , result->getString("entryTime").c_str());
		
			res.push_back(tmp);
		}
		delete result;
		return res;
	};
/*private:
	QSqlDatabase dbconn*/;
};