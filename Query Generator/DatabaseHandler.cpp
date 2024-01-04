// DatabaseHandler.cpp

#include "DatabaseHandler.h"
#include "QueryFileHandler.h"

DatabaseHandler::DatabaseHandler(const std::string& host, const std::string& user, const std::string& password)
    : driver(nullptr), con(nullptr), host_(host), user_(user), password_(password) {}

DatabaseHandler::~DatabaseHandler() {
    delete con;
}

bool DatabaseHandler::connect() {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://" + host_ + ":3306", user_, password_);
        std::cout << "Connection successful!" << std::endl;
        return true;
    } catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        std::cerr << "MySQL error code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;

        std::cout << "Connection unsuccessful!" << std::endl;
        return false;
    }
}

void DatabaseHandler::executeQueriesFromFile(const std::string& fileName) {
    std::ifstream queryFile(fileName);
    if (queryFile.is_open()) {
        std::vector<std::string> queries;
        std::string query;

        while (std::getline(queryFile, query)) {
            queries.push_back(query);
        }

        queryFile.close();

        // Execute queries using QueryFileHandler
        if (QueryFileHandler::executeQueriesFromFile(queries, con)) {
            std::cout << "All queries executed successfully." << std::endl;
        } else {
            std::cerr << "Some queries failed to execute." << std::endl;
        }
    } else {
        std::cerr << "Error: Could not open query file." << std::endl;
    }
}