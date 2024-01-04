#include "DatabaseHandler.h"
#include "QueryGenerator.h"
#include "LogFileHandler.h"
#include "QueryFileHandler.h"

#include <thread>  // for std::this_thread::sleep_for

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " --user <username> --password <password>" << std::endl;
        return 1;
    }

    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; i += 2) {
        args[argv[i]] = argv[i + 1];
    }

    if (args.find("--user") == args.end() || args.find("--password") == args.end()) {
        std::cerr << "Invalid arguments. Usage: " << argv[0] << " --user <username> --password <password>" << std::endl;
        return 1;
    }

    const std::string host = "127.0.0.1";
    const std::string user = args["--user"];
    const std::string password = args["--password"];

    DatabaseHandler dbHandler(host, user, password);

    if (dbHandler.connect()) {

        LogFileHandler::clearLogFile("output/log.txt");
        QueryFileHandler::clearQueryFile("output/queries.txt");

        if (LogFileHandler::createLogFile("log.txt")) {
            // Generate queries and store them in a file
            std::string queryFileName = "output/queries.txt";
            std::ofstream queryFile(queryFileName);

            if (QueryFileHandler::createQueryFile(queryFileName)) {
                std::vector<std::string> queries;

                for (int i = 0; i < 1; ++i) {
                    std::string dbName = "database_" + std::to_string(i);
                    queries.push_back(QueryGenerator::generateDropDatabaseQuery(dbName));

                    QueryFileHandler::clearQueryFile(queryFileName);
                    QueryFileHandler::createQueryFile(queryFileName);

                    // Generate queries and add them to the vector
                    queries.push_back(QueryGenerator::generateCreateDatabaseQuery(dbName));
                    queries.push_back(QueryGenerator::generateUseDatabaseQuery(dbName));

                    for (int j = 0; j < 5; ++j) {
                        std::string tableName = "table_" + std::to_string(j);
                        queries.push_back(QueryGenerator::generateCreateTableQuery(tableName));

                        for (int k = 0; k < 5; ++k) {
                            int randomId = rand() % 1000;
                            std::string randomName = "Name_" + std::to_string(randomId);
                            queries.push_back(QueryGenerator::generateInsertQuery(tableName, randomId, randomName));
                        }

                        queries.push_back(QueryGenerator::generateAdvancedQuery(tableName));
                    }
                }

                // Write the queries to the file
                QueryFileHandler::writeQueriesToFile(queryFileName, queries);
            }

            std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait for 5 seconds

            // Read and execute queries from the file
            LogFileHandler::startTimer();
            dbHandler.executeQueriesFromFile(queryFileName);
            LogFileHandler::stopTimer();

            LogFileHandler::log("log.txt", "Total execution time: " + std::to_string(LogFileHandler::getElapsedTime()) + " seconds");

            LogFileHandler::closeLogFile("log.txt");
        }
    }

    return 0;
}
