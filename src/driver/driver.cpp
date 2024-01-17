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
        for (driver::Patch patch : *patches)
        {
            // std::cout << patch.getValue() << std::endl;
            auto startTime = std::chrono::high_resolution_clock::now();
            clusterAccess->patch(&initialConfig, NULL, "default", patch);
            while (!clusterAccess->isPropagationComplete(initialConfig, &patch))
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            patch.setScore(duration.count());
            std::cout << "Time duration: " << duration.count() << " milliseconds" << std::endl;
        }
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
    // query generator and kubernetes service connector

    myThread.join();

    cJSON_Delete(initialSetup);
    return 0;
}