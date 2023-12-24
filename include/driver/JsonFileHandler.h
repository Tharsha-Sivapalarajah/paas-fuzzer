#pragma once

#include <iostream>
#include <iostream>
#include <vector>
#include <optional>
#include <json/json.h>
#include "Patch.h"

namespace driver
{
    class JsonFileHandler
    {
    public:
        bool readJsonFile(Json::Value &jsonData, const std::string &inputFilePath) const;
        bool createConfigFile(Json::Value &configJsonData, std::vector<Patch> &patches);
        bool manipulateJsonData(Json::Value jsonData);
        bool writeJsonFile(const Json::Value &jsonData, const std::string &outputFilePath);

    private:
        std::optional<std::vector<std::string>> handleJsonObj(Json::Value &jsonObject, std::vector<std::string> keys, std::size_t keysSize, std::vector<std::vector<std::string>> &patchableKeys);
    };
};