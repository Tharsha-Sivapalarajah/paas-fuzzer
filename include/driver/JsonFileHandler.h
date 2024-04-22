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
#include <random>
#include <filesystem>

#include "Patch.h"
#include "constant.h"
#include "Evaluator.h"

extern "C"
{
#include <external/cJSON.h>
}

namespace fs = std::filesystem;

namespace driver
{
    class JsonFileHandler
    {
    public:
        cJSON *createJSONObject(const char *filename) const;
        cJSON *createConfigFile(const cJSON *inputJson, std::vector<std::string> keys, std::vector<std::vector<std::string>> &patchableKeys);
        /**
         * @brief reads the JSON input file from the given location and return the JSON values into Json::Value
         *
         * @param jsonData Reference to the Json::Value object where the read JSON data will be stored.
         * @param inputFilePath Path location of the input JSON configuration file.
         * @return true if read successful false otherwise.
         * @return false
         */
        bool readJsonFile(Json::Value &jsonData, const std::string &inputFilePath) const;

        /**
         * @brief Use the given configuration file data in Json::Value format to create patches from the configuration file and store the patches in the `patches` argument.
         *
         * @param configJsonData Configuration file information in the Json::Value format.
         * @param patches A reference vector of Patch objects which will be generated from the given Configuration JSON file.
         * @return true if the creating patches from the configuration file is success
         * @return false otherwise
         */
        // bool createConfigFile(Json::Value &configJsonData, std::vector<driver::Patch> &patches);

        /**
         * @brief Generate a cJSON object for the given Json::Value object.
         *
         * @param jsonFile A referece parameter to the input Json::Value object
         * @return cJSON* This contains the values in the input Json::Value object
         */
        cJSON *generateCJSON(const Json::Value &jsonFile) const;

        /**
         * @brief Print the cJSON object into string to view in pretty.
         *
         * @param cJsonObject Input cJSON file to print
         */
        static void printCJSON(const cJSON *cJsonObject);

        // bool manipulateJsonData(Json::Value jsonData);

        /**
         * @brief Write the Json::Value to the provided output path location
         *
         * @param jsonData Json::Value object which contain the information to write to the file
         * @param outputFilePath output path location of the file including the file name
         * @return true if the writing is successful
         * @return false otherwise
         */
        bool writeJsonFile(const Json::Value &jsonData, const std::string &outputFilePath);

        bool reportBug(cJSON *previousConfigFile, cJSON *currentConfigFile, std::vector<driver::Patch> &patches, driver::Patch &currentPatch, int verbose);

        void modifyPatch(cJSON *jsonData, driver::Patch patch) const;

        bool compareCJSONObjects(cJSON *obj1, cJSON *obj2, std::vector<std::string> &keys);

        bool isPatchIncluded(cJSON *jsonData, Patch &patch, int verbose) const;

    private:
        static bool bugReportDirectoryCreated;

        char *readJSONFile(const char *filename) const;

        bool isInteger(const std::string &s) const;
        void jsoncppToCJSON(const Json::Value &jsonValue, cJSON *cjsonObject) const;
        int handleJsonObj(Json::Value &jsonObject, std::vector<std::string> keys, std::size_t keysSize, std::vector<std::vector<std::string>> &patchableKeys, Json::Value &parentJson);
        std::string generateRandomFileName() const;
    };
};