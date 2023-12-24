#include <iostream>
#include <fstream>
#include <json/json.h>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "JsonFileHandler.h"
#include "Patch.h"
#include "constant.h"

namespace driver
{
    bool JsonFileHandler::readJsonFile(Json::Value &jsonData, const std::string &inputFilePath) const
    {
        std::ifstream input_file(inputFilePath);

        if (!input_file.is_open())
        {
            std::cerr << "Error opening input JSON file." << std::endl;
            return false;
        }

        input_file >> jsonData;
        input_file.close();

        return true;
    }

    bool JsonFileHandler::createConfigFile(Json::Value &configJsonData, std::vector<Patch> &patches)
    {
        std::vector<std::vector<std::string>> patchableKeys;

        for (const auto &key : configJsonData.getMemberNames())
        {
            // std::cout << element << std::endl;
            std::string currentKey = key;
            std::vector<std::string> keys = {};
            keys.push_back(currentKey);

            Json::Value &element = configJsonData[currentKey];

            handleJsonObj(element, keys, keys.size(), patchableKeys);
        }

        if (patchableKeys.size() > 0)
        {
            for (std::vector<std::string> patchKey : patchableKeys)
            {
                Patch patch(patchKey, 0.0);
                patches.push_back(patch);
            }
            return true;
        }
        return false;
        // for (const auto &row : patchableKeys)
        // {
        //     for (const auto &element : row)
        //     {
        //         std::cout << element << " ";
        //     }
        //     std::cout << std::endl;
        // }
    }

    std::optional<std::vector<std::string>> JsonFileHandler::handleJsonObj(Json::Value &jsonObject, std::vector<std::string> keys, std::size_t keysSize, std::vector<std::vector<std::string>> &patchableKeys)
    {
        if (jsonObject.isObject())
        {
            // std::cout << jsonObject << std::endl;
            std::vector<std::string> tempKeys = jsonObject.getMemberNames();
            // std::cout << tempKeys.size() << std::endl;
            std::string currentKey;

            for (int i = 0; i < tempKeys.size(); i++)
            {
                std::vector<std::string> currentKeys = keys;
                currentKey = tempKeys[i];
                Json::Value &element = jsonObject[currentKey];
                // std::cout << element << std::endl;
                currentKeys.push_back(currentKey);

                if (handleJsonObj(element, currentKeys, keys.size(), patchableKeys).has_value())
                {
                    if (std::find(driver::forbiddenPatchKeyStrings.begin(), driver::forbiddenPatchKeyStrings.end(), currentKeys[currentKeys.size() - 1]) == driver::forbiddenPatchKeyStrings.end())
                    { // not found
                        patchableKeys.push_back(currentKeys);
                    }
                }
            }
            return std::nullopt;
        }
        else if (jsonObject.isArray())
        {
            bool isPatch = true;
            for (int i = 0; i < jsonObject.size(); i++)
            {
                Json::Value arrayValue = jsonObject[i];
                if (!arrayValue.isString() && !arrayValue.isInt())
                { // not patch
                    isPatch = false;
                    if (arrayValue.isObject())
                    {
                        std::vector<std::string> currentKeys = keys;
                        currentKeys.push_back(std::to_string(i + 1));
                        if (handleJsonObj(arrayValue, currentKeys, currentKeys.size(), patchableKeys))
                        {
                            std::cout << jsonObject << std::endl;

                            if (std::find(driver::forbiddenPatchKeyStrings.begin(), driver::forbiddenPatchKeyStrings.end(), currentKeys[currentKeys.size() - 1]) == driver::forbiddenPatchKeyStrings.end())
                            { // not found
                                patchableKeys.push_back(currentKeys);
                            }
                        }
                    }
                }
            }

            if (isPatch == true)
            {
                return keys;
            }

            return std::nullopt;
        }
        return std::nullopt;
    }

    //     bool writeJsonFile(const Json::Value &jsonData, const std::string &outputFilePath)
    //     {
    //         std::ofstream output_file(outputFilePath);
    //         if (!output_file.is_open())
    //         {
    //             std::cerr << "Error opening output JSON file." << std::endl;
    //             return false;
    //         }

    //         // Use setw from iomanip for formatting
    //         output_file << std::setw(4) << jsonData << std::endl;
    //         output_file.close();

    //         return true;
    //     }

    // private:
    //     int generateRandomNumber(int minValue, int maxValue)
    //     {
    //         // Seed the random number generator with the current time
    //         std::srand(static_cast<unsigned>(std::time(nullptr)));

    //         // Generate a random number between minValue and maxValue (inclusive)
    //         return std::rand() % (maxValue - minValue + 1) + minValue;
    //     }

};