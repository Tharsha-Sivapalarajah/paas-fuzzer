#include <iostream>
#include <fstream>
#include <json/json.h>
#include <iomanip>
#include <filesystem>

#include "JsonFileHandler.h"
#include "Patch.h"

using namespace driver;

int main()
{
    // Your main program logic here
    JsonFileHandler fileHandler;

    // Read JSON file
    Json::Value originalData;
    fileHandler.readJsonFile(originalData, "../data/input.json");

    Json::Value configFile = originalData;
    std::vector<Patch> patches = {};
    fileHandler.createConfigFile(configFile, patches);

    for (Patch patch : patches)
    {
        std::vector<std::string> patchKey = patch.getKeys();
        for (const auto &element : patchKey)
        {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
