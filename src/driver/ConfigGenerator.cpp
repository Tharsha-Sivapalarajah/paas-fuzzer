#include <iostream>
#include <fstream>
#include <json/json.h>
#include <iomanip>
#include <filesystem>

#include "JsonFileHandler.h"
#include "Patch.h"
#include "ConfigGenerator.h"

using namespace driver;

namespace driver
{

}

int main()
{
    JsonFileHandler fileHandler;

    // Read JSON file
    Json::Value originalData;
    fileHandler.readJsonFile(originalData, "../../../data/output.json");

    // std::cout << originalData.getMemberNames()[0] << std::endl;

    cJSON *value = fileHandler.generateCJSON(originalData);
    // cJSON *name = cJSON_GetObjectItemCaseSensitive(value, "apiVersion");
    // cJSON_Delete(value);
    // cJSON_free(name);
    // char *val = cJSON_Print(name);
    // printf("%s\n", val);
    Json::Value configFile = originalData;
    std::vector<Patch> patches = {};
    fileHandler.createConfigFile(configFile, patches);

    std::string patchPatch="";
    // Patch::getStringFromPatches(patches,);

    // fileHandler.writeJsonFile(configFile, "../data/output.json");

    // for (Patch patch : patches)
    // {
    //     std::vector<std::string> patchKey = patch.getKeys();
    //     for (const auto &element : patchKey)
    //     {
    //         std::cout << element << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // return 0;
}
