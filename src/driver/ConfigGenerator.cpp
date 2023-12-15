#include <iostream>
#include <fstream>
#include <json/json.h>
#include <iomanip>
#include <filesystem>

#include <JsonFileHandler.h>

int main()
{
    JsonFileHandler fileHandler("../../data/input.json");

    // Read JSON file
    Json::Value originalData;
    fileHandler.readJsonFile(originalData);

    Json::Value configFile = originalData;
    fileHandler.createConfigFile(configFile);

    return 0;
}
