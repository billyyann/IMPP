#pragma once
#include <cstdint>
#include <string>
#include <stdio.h>
#include <vector>
#include <sstream>


typedef struct _Task {
    char uuid[36];
    uint32_t cycleId;
    uint32_t satId;
    uint32_t payloadId;
    uint32_t waveBandId;
    float integralTime;
    char startTimeStamp[20];
    char endTimeStamp[20];
    char entryTime[20];
    uint16_t status; //0 pending  1 running  2 done 3 fail

    std::vector<std::string> getKeys() {
        return { "cycleId","satId","payloadId","waveBandId","integralTime","startTimeStamp","endTimeStamp","entryTime","status"};
    };

    std::string select() {
        char sql[2048];		// ¥Ê¥¢sql”Ôæ‰
        snprintf(sql, 2048, "select * from  task where cycleId=%d;", cycleId);
        return sql;
    };
  
    std::string insert() {
        char sql[2048];		// ¥Ê¥¢sql”Ôæ‰
        snprintf(sql, 2048, "INSERT INTO task (UUID,CYCLEID,SATID,PAYLOADID,WAVEBANDID,INTEGRALTIME,STARTTIMESTAMP,ENDTIMESTAMP,ENTRYTIME,STATUS) \
                VALUES(UUID(),%d, %d, %d, %d,%f, '%s', '%s',CURRENT_TIMESTAMP(),%d);",
            cycleId, satId, payloadId, waveBandId, integralTime, &startTimeStamp[0], &endTimeStamp[0], status);
        return sql;
    };
    std::string del() {
        char sql[2048];		// ¥Ê¥¢sql”Ôæ‰
        snprintf(sql, 2048, "delete from task where cycleId=%d;", cycleId);
        return sql;
    }
    std::string update() {
        char sql[2048];		// ¥Ê¥¢sql”Ôæ‰
        snprintf(sql, 2048, "UPDATE task SET status = % d WHERE satId = % d ;", status,cycleId);
        return sql;
    };
}Task;

typedef struct _GenericImage{
    char uuid[36];
    uint32_t processStationId;
    uint32_t taskId;
    uint32_t satId;
    uint32_t payloadId;
    uint32_t waveBandId;
    uint32_t channelId;
    double integralTime;
    char startTimeStamp[20]; //YYYY-MM-DD HH::MM::SS
    char endTimeStamp[20];
    uint16_t startFrameCount;
    uint16_t endFrameCount;
    uint16_t stareWorkMode;
    char gentime[20];
    uint16_t subFileLen;
    uint16_t subFileNum;
    char remoteFilePath[255]; 
    char localFilePath[255];
    uint16_t lifeStatus; //0 extracted 1 deployed
    char entryTime[20];
    uint16_t payloadStatus;

    std::vector<std::string> getKeys() {
        return { "taskId","satId","payloadId","waveBandId","integralTime","startTimeStamp","startFrameCount","endFrameCount",
            "endTimeStamp","remoteFilePath","localFilePath" ,"lifeStatus"};
    };

    std::string select() {
        char sql[2048];		// ¥Ê¥¢sql”Ôæ‰
        snprintf(sql, 2048, "select * from  genericimage where taskId=%d;", taskId);
        return sql;
    };

    std::string insert() {
        std::stringstream ss;
        char sql[2048];		// ¥Ê¥¢sql”Ôæ‰
        ss << "INSERT INTO genericimage \
                    (UUID,PROCESSSTATIONID,TASKID,SATID,PAYLOADID,WAVEBANDID,INTEGRALTIME,CHANNELID,STARTTIMESTAMP,ENDTIMESTAMP,\
                    STARTFRAMECOUNT,ENDFRAMECOUNT,STAREWORKMODE,GENTIME,SUBFILELEN,SUBFILENUM,REMOTEFILEPATH,LOCALFILEPATH,LIFESTATUS,ENTRYTIME,PAYLOADSTATUS) \
                    VALUES( UUID()," << processStationId << ',' << taskId << ',' << satId << ',' << payloadId << ',' << waveBandId
            << ',' << integralTime << ',' << channelId << ",'" << std::string(startTimeStamp) << "','" << std::string(endTimeStamp) << "',"
            << startFrameCount << ',' << endFrameCount << ',' << stareWorkMode << ",'" << gentime << "'," << subFileLen << ',' << subFileNum << ",'" << std::string(remoteFilePath) << "','" <<
            std::string(localFilePath) << "'," << lifeStatus << ',' << "CURRENT_TIMESTAMP()," << payloadStatus << ");";
        return ss.str();
    };
    std::string update() {
        char sql[2048];
        snprintf(sql, 2048, "UPDATE genericimage SET lifeStatus = %d WHERE satId = %d and  payloadId = %d and  waveBandId = %d and  startFrameCount = %d;",
            lifeStatus, satId, payloadId, waveBandId, startFrameCount);
        return sql;
    }
    std::string del() {};
} GenericImage;

typedef struct _ProcessPattern {
    char uuid[36];
    uint32_t satId;
    uint32_t payloadId;
    uint32_t waveBandId;
    double integralTime;
    char module[64];
    char algorithm[64];
    char parameters[1024];
    char condition[1024];
    char entryTime[20];
    int16_t status;

    std::vector<std::string> getKeys() {
        return { "satId","payloadId","waveBandId","integralTime","module","algorithm","parameters","condition","entryTime"};
    }

    std::string select() {
        char sql[2048];		// ¥Ê¥¢sql”Ôæ‰
        snprintf(sql, 2048, "select * from  processpattern where module=%d and algorithm=%d;",module, algorithm);
        return sql;
    };

    std::string insert() {
        char sql[2048];
        snprintf(sql, 2048, "INSERT INTO processpattern ( SATID, PAYLOADID, WAVEBANDID, INTEGRALTIME, MODULE, ALGORITHM, PARAMETERS, CONDITIONS, ENTRYTIME) \
                VALUES(%d,%d,%d,%f,%d,%d,'%s','%s',CURRENT_TIMESTAMP());",
            satId, payloadId, waveBandId, integralTime,module, algorithm, parameters, condition);
        return sql;
    }
    std::string update() {};
    std::string del() {};
}ProcessPattern;


typedef struct _ProcessRecord {
    char uuid[36];
    uint32_t genericImageId;
    uint32_t taskId;
    char localFilePath[256];
    char entryTime[20];

    std::vector<std::string> getKeys() {
        return { "genericImageId","processPatternId","entryTime"};
    }

    std::string select() {
        char sql[2048];		// ¥Ê¥¢sql”Ôæ‰
        snprintf(sql, 2048, "select * from  processrecord where GENERICIMAGEID=%d;", genericImageId);
        return sql;
    };

    std::string insert() {
        char sql[2048];
        snprintf(sql, 2048, "INSERT INTO processrecord  (UUID,GENERICIMAGEID,TASKID,LOCALFILEPATH,ENTRYTIME)  VALUES(UUID(),%d,%d,%s,CURRENT_TIMESTAMP());"
            , genericImageId, taskId,localFilePath);
        return sql;
    }
    std::string update() {};
    std::string del() {};
}ProcessRecord;


typedef struct _SubTask {
    char uuid[36];
    char taskId[36];
    char processPatternId[36];
    char step[36];
    int status; //0 pending 1 running 2 done 3  fail
    char entryTime[20];
    std::vector<std::string> getKeys() {
        return { "taskId","processPatternId","step","entryTime"};
    }

    std::string select() {
        char sql[2048];		// ¥Ê¥¢sql”Ôæ‰
        snprintf(sql, 2048, "select * from  subtask where status=%d;", 0);
        return sql;
    };

    std::string insert() {
        char sql[2048];
        snprintf(sql, 2048, "INSERT INTO SUBTASK (UUID,taskId,processPatternId,step,status,entryTime)  VALUES(UUID(),%s,%s,%s,%d,CURRENT_TIMESTAMP());"
            , taskId, processPatternId, step, status);
        return sql;
    }

        std::string update() {};
    std::string del() {};

} SubTask;