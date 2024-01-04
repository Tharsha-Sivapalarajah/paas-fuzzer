#include "driver.h"

using namespace driver;

bool compareByFloatValue(const driver::Patch &obj1, const driver::Patch &obj2)
{
    return obj1.getScore() > obj2.getScore();
}

void updatePatches(std::vector<driver::Patch> *patches, scheduler::ClusterAccess *clusterAccess, cJSON *initialConfig)
{
    while (true)
    {
        // for (driver::Patch patch : *patches)
        // {
        // std::cout << patch.getValue() << std::endl;
        clusterAccess->patch(initialConfig, NULL, "default", (*patches)[2]);
        // }
        std::sort((*patches).begin(), (*patches).end(), compareByFloatValue);
        break;
    }
}

int main()
{
    driver::JsonFileHandler fileHandler;

    // Read the initila JSON input file
    Json::Value originalData;
    fileHandler.readJsonFile(originalData, "../../../data/input.json");

    Json::Value configFile = originalData;
    std::vector<driver::Patch> patches = {};
    fileHandler.createConfigFile(configFile, patches);

    cJSON *initialSetup = fileHandler.generateCJSON(configFile);

    scheduler::ClusterAccess clusterAccess;
    if (clusterAccess.create(
            initialSetup,
            NULL,
            "default"))
    {
        std::cout << "Initial Application Setup Completed" << std::endl;
    }
    else
    {
        // return 1;
    }

    std::thread myThread(updatePatches, &patches, &clusterAccess, initialSetup);

    // Do some work in the main thread

    myThread.join(); // Wait for the thread to finish

    cJSON_Delete(initialSetup);
    return 0;
}