#include "DatabaseHandler.h"
#include "QueryGenerator.h"
#include "LogFileHandler.h"
#include "QueryFileHandler.h"


#include <sstream>
#include <thread>  // for std::this_thread::sleep_for
#include <cppconn/statement.h> 

namespace QueryGeneration {}

// Function to read credentials from the file
std::map<std::string, std::string> readCredentials(const std::string& filename) {
    std::map<std::string, std::string> credentials;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string key, value;
            if (iss >> key >> value) {
                credentials[key] = value;
            }
        }
        file.close();
    }

    return credentials;
}

int main(int argc, char *argv[]) {
    // Read credentials from the file
    std::map<std::string, std::string> credentials = readCredentials("credentials.txt");

    // Check if all required credentials are present
    if (credentials.find("host") == credentials.end() ||
        credentials.find("port") == credentials.end() ||
        credentials.find("user") == credentials.end() ||
        credentials.find("password") == credentials.end()) {
        std::cerr << "Invalid or incomplete credentials in the file." << std::endl;
        return 1;
    }

    const std::string host = credentials["host"];
    const std::string port = credentials["port"];
    const std::string user = credentials["user"];
    const std::string password = credentials["password"];

    QueryGeneration :: DatabaseHandler dbHandler(host, port, user, password);

    if (dbHandler.connect()) {

        QueryGeneration :: LogFileHandler::clearLogFile("output/log.txt");
        QueryGeneration :: QueryFileHandler::clearQueryFile("output/queries.txt");

        if (QueryGeneration :: LogFileHandler::createLogFile("log.txt")) {
            // Generate queries and store them in a file
            std::string queryFileName = "output/queries.txt";
            std::ofstream queryFile(queryFileName);

            if (QueryGeneration :: QueryFileHandler::createQueryFile(queryFileName)) {
                std::vector<std::string> queries;

                for (int i = 0; i < 1; ++i) {
                    std::string dbName = "database_" + std::to_string(i);
                    queries.push_back(QueryGeneration::QueryGenerator::generateDropDatabaseQuery(dbName));

                    QueryGeneration::QueryFileHandler::clearQueryFile(queryFileName);
                    QueryGeneration::QueryFileHandler::createQueryFile(queryFileName);

                    // Generate queries and add them to the vector
                    queries.push_back(QueryGeneration::QueryGenerator::generateCreateDatabaseQuery(dbName));
                    queries.push_back(QueryGeneration::QueryGenerator::generateUseDatabaseQuery(dbName));

                    for (int j = 0; j < 5; ++j) {
                        std::string tableName = "table_" + std::to_string(j);
                        queries.push_back(QueryGeneration::QueryGenerator::generateCreateTableQuery(tableName));

                        for (int k = 0; k < 5; ++k) {
                            int randomId = rand() % 1000;
                            std::string randomName = "Name_" + std::to_string(randomId);
                            queries.push_back(QueryGeneration::QueryGenerator::generateInsertQuery(tableName, randomId, randomName));
                        }

                        // Add SELECT, WHERE, ORDER BY, and JOIN queries
                        std::vector<std::string> columnsToSelect = {"field1", "field2", "field3"};
                        queries.push_back(QueryGeneration::QueryGenerator::generateSelectQuery(tableName, columnsToSelect));

                        std::string whereCondition = "field2 > 50";
                        queries.push_back(QueryGeneration::QueryGenerator::generateWhereQuery(tableName, whereCondition));

                        std::string orderByColumn = "field3 DESC";
                        queries.push_back(QueryGeneration::QueryGenerator::generateOrderByQuery(tableName, orderByColumn));

                        if (j != 0){
                            std::string joinCondition = "table_0.id = table_" + std::to_string(j) + ".id";
                            queries.push_back(QueryGeneration::QueryGenerator::generateJoinQuery("table_0", tableName, joinCondition));
                        }
                    }
                }


                // Write the queries to the file
                QueryGeneration :: QueryFileHandler::writeQueriesToFile(queryFileName, queries);
            }

            std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait for 5 seconds

            dbHandler.enableQueryProfiling();
            dbHandler.executeQueriesFromFile(queryFileName);  // Read and execute queries from the file
            double totalExecutionTime = dbHandler.getTotalExecutionTime();

            QueryGeneration :: LogFileHandler::log("log.txt", "Total execution time in Local Machine: " + std::to_string(totalExecutionTime) + " seconds");
            QueryGeneration :: LogFileHandler::closeLogFile("log.txt");
        }
    }

    return 0;
}
