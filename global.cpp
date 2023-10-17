#include "global.h"

std::shared_ptr<sql::mysql::MySQL_Driver> g_driver{nullptr};
 std::shared_ptr<sql::Connection> g_con{nullptr};
 std::shared_ptr <sql::Statement> g_stmt{nullptr};
 std::map < std::string, std::vector<std::string>> g_datcache;
 ThreadPool g_pool(4);