#include "JwsMainWindow.h"
//#include <QtWidgets/QApplication>
#define  _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 
#include "global.h"
#include "RcConfig.h"


bool static connectMysql()
{
	jws::RcConfig config;
	bool ret = config.ReadConfig("cfg.ini");
	if (ret == false) {
		printf("ReadConfig is Error,Cfg=%s", "cfg.ini");
		return false;
	}
	std::string db = config.ReadString("MYSQL", "DB", "dat37");
	std::string ip = config.ReadString("MYSQL", "IP", "127.0.0.1");
	int port = config.ReadDouble("MYSQL", "PORT", 3306);
	std::string user = config.ReadString("MYSQL", "USER", "root");
	std::string password = config.ReadString("MYSQL", "PASSWORD", "1234");

	g_driver.reset(sql::mysql::get_mysql_driver_instance());
	
	sql::ConnectOptionsMap connection_properties;
	connection_properties["hostName"] = "tcp://127.0.0.1:3306";
	connection_properties["userName"] = "root";
	connection_properties["password"] = "1234";
	connection_properties["schema"] = "dat37";
	connection_properties["port"] = 3306;

	g_con.reset(g_driver.get()->connect(connection_properties));
	g_stmt.reset(g_con.get()->createStatement());

	g_stmt->execute("use "+db);
	return true;
};

#define TEST 1


#if TEST
#include "TaskScheduleService.h"
void testcase1_repository_task_insert() {

	connectMysql();
    auto dpt=TaskScheduleService::GetInstance();
    TaskHeader task{0,5122,1,3};
    double IT = 20;
    std::string startTime = "2023-09-15 11:11:11";
	std::string endTime = "2023-09-15 11:11:11";

    //dpt->addTask(task,IT,startTime,endTime);
}

void testcase1_repository_genericImage_insert() {

	connectMysql();
	auto dpt= GenericImageService::GetInstance();
	std::string fn = "C:/Users/86138/Desktop/project/zdg_calibration/data/data/ECC/C/DPD/HY1403_20230609_00000006_LGXJ_02_DPD1.dat";
	RawImageIO rawImage;
	 dpt->loadImage( fn, IMAGELEVEL::ONE);
}


void testcase1_repository_genericImage_insert2() {

	connectMysql();
	auto dpt = GenericImageService::GetInstance();
	std::string fn = "C:/Users/86138/Desktop/project/zdg_calibration/data/data/ECC/C/DPD/HY1403_20230609_00000006_LGXJ_02_DPD1.dat";
	RawImageIO rawImage;
	 dpt->loadImage( fn, IMAGELEVEL::ONE);
}


int main(int argc, char* argv[]) {
    //QCoreApplication a(argc, argv);
    //testcase1_repository_task_insert();
	//testcase1_repository_genericImage_insert();
}
#else
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JwsMainWindow w;
    w.show();
    return a.exec();

#endif
