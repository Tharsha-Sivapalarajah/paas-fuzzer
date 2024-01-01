#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include <json/json.h>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "Patch.h"
#include "constant.h"

extern "C"
{
#include <external/cJSON.h>
}

namespace driver
{
    class JsonFileHandler
    {
    public:
        bool readJsonFile(Json::Value &jsonData, const std::string &inputFilePath) const;
        bool createConfigFile(Json::Value &configJsonData, std::vector<driver::Patch> &patches);
        cJSON *generateCJSON(const Json::Value &jsonFile) const;
        void printCJSON(const cJSON *cJsonObject);
        bool manipulateJsonData(Json::Value jsonData);
        bool writeJsonFile(const Json::Value &jsonData, const std::string &outputFilePath);

    private:
        void jsoncppToCJSON(const Json::Value &jsonValue, cJSON *cjsonObject) const;
        std::optional<std::vector<std::string>> handleJsonObj(Json::Value &jsonObject, std::vector<std::string> keys, std::size_t keysSize, std::vector<std::vector<std::string>> &patchableKeys);
    };
};