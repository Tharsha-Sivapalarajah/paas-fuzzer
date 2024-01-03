#include "driver.h"

using namespace driver;

bool compareByFloatValue(const Patch &obj1, const Patch &obj2)
{
    return obj1.getScore() > obj2.getScore();
}

void updatePatches(std::vector<Patch> *patches, scheduler::ClusterAccess *clusterAccess, cJSON *initialConfig)
{
    while (true)
    {
        for (Patch &patch : *patches)
        {
            clusterAccess->patch(initialConfig, NULL, "default", patch);
        }
        std::sort((*patches).begin(), (*patches).end(), compareByFloatValue);
    }
}

int main()
{
    JsonFileHandler fileHandler;

    // Read the initila JSON input file
    Json::Value originalData;
    fileHandler.readJsonFile(originalData, "../../../data/input.json");

    Json::Value configFile = originalData;
    std::vector<Patch> patches = {};
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
        return 1;
    }

    std::thread myThread(updatePatches);

    // Do some work in the main thread

    myThread.join(); // Wait for the thread to finish

    return 0;
}