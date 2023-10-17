#pragma once
#include <memory>
#include <mysql/jdbc.h>
#include <string>
#include <vector>
#include "ThreadPool.h"


extern std::shared_ptr<sql::mysql::MySQL_Driver> g_driver;
extern std::shared_ptr<sql::Connection> g_con;
extern std::shared_ptr <sql::Statement> g_stmt;

extern std::map < std::string, std::vector<std::string>> g_datcache;

extern ThreadPool g_pool;








