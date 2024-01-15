#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>

namespace QueryGeneration {
    class DatabaseHandler {
    public:
        DatabaseHandler(const std::string& host, const std::string& user, const std::string& password);
        ~DatabaseHandler();

        bool connect();
        void executeQueriesFromFile(const std::string& fileName);

    private:
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;
        std::string host_;
        std::string user_;
        std::string password_;
    };
}
#endif // DATABASE_HANDLER_H
