// QueryGenerator.h

#ifndef QUERY_GENERATOR_H
#define QUERY_GENERATOR_H

#include <string>

class QueryGenerator {
public:
    static std::string generateDropDatabaseQuery(const std::string& dbName);
    static std::string generateCreateTableQuery(const std::string& tableName);
    static std::string generateInsertQuery(const std::string& tableName, int id, const std::string& name);
    static std::string generateSelectQuery(const std::string& tableName);
    static std::string generateAdvancedQuery(const std::string& tableName);
    static std::string generateCreateDatabaseQuery(const std::string& dbName);
    static std::string generateUseDatabaseQuery(const std::string& dbName);
};

#endif // QUERY_GENERATOR_H
