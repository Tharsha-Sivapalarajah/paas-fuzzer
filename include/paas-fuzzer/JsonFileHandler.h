#ifndef JSONFILEREADER_H
#define JSONFILEREADER_H

#include <iostream>
#include <json/json.h>

class JsonFileHandler
{
public:
    JsonFileHandler(const std::string &inputFilePath);
    bool readJsonFile(Json::Value &jsonData);
    bool createConfigFile(Json::Value configJsonData);
    bool manipulateJsonData(Json::Value jsonData);
    bool writeJsonFile(const Json::Value &jsonData, const std::string &outputFilePath);
};

#endif