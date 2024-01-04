#include "JsonFileHandler.h"

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

            int response = handleJsonObj(element, keys, keys.size(), patchableKeys, configJsonData);
        }

        if (patchableKeys.size() > 0)
        {
            for (std::vector<std::string> patchKey : patchableKeys)
            {
                if (!patchKey.empty())
                {
                    std::string value = patchKey.back();
                    patchKey.pop_back();

                    Patch patch(patchKey, 0.0, value);
                    patches.push_back(patch);
                }
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

    int JsonFileHandler::handleJsonObj(Json::Value &jsonObject, std::vector<std::string> keys, std::size_t keysSize, std::vector<std::vector<std::string>> &patchableKeys, Json::Value &parentJson)
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

                int response = handleJsonObj(element, currentKeys, keys.size(), patchableKeys, jsonObject);
            }
            return 0;
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
                        int response = handleJsonObj(arrayValue, currentKeys, currentKeys.size(), patchableKeys, jsonObject);
                    }
                }
            }

            if (isPatch == true)
            {
                if (std::find(driver::forbiddenPatchKeyStrings.begin(), driver::forbiddenPatchKeyStrings.end(), keys[keys.size() - 1]) == driver::forbiddenPatchKeyStrings.end())
                { // not found
                    for (int i = 0; i < jsonObject.size(); i++)
                    {
                        std::vector<std::string> currentKeys = keys;
                        Json::Value arrayValue = jsonObject[i];
                        currentKeys.push_back(jsonObject[i].asString());
                        patchableKeys.push_back(currentKeys);
                    }
                }
                parentJson[keys.back()] = jsonObject[0];
                return 1;
            }
        }
        return 0;
    }

    bool JsonFileHandler::writeJsonFile(const Json::Value &jsonData, const std::string &outputFilePath)
    {
        std::ofstream output_file(outputFilePath);
        if (!output_file.is_open())
        {
            std::cerr << "Error opening output JSON file." << std::endl;
            return false;
        }

        // Use setw from iomanip for formatting
        output_file << std::setw(4) << jsonData << std::endl;
        output_file.close();

        return true;
    }

    int generateRandomNumber(int minValue, int maxValue)
    {
        // Seed the random number generator with the current time
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        // Generate a random number between minValue and maxValue (inclusive)
        return std::rand() % (maxValue - minValue + 1) + minValue;
    }

    void JsonFileHandler::jsoncppToCJSON(const Json::Value &jsonValue, cJSON *cjsonObject) const
    {
        // Iterate through JSON object
        std::vector<std::string> keys = jsonValue.getMemberNames();
        for (std::string &key : keys)
        {
            const Json::Value &value = jsonValue[key];
            // Determine the type of JSON value and convert to CJSON
            if (value.isObject())
            {
                // Recursive call for nested objects
                cJSON *nestedObject = cJSON_CreateObject();
                cJSON_AddItemToObject(cjsonObject, key.c_str(), nestedObject);
                jsoncppToCJSON(value, nestedObject);
            }
            else if (value.isArray())
            {
                cJSON *array = cJSON_CreateArray();
                cJSON_AddItemToObject(cjsonObject, key.c_str(), array);

                for (const auto &element : value)
                {
                    if (element.isObject())
                    {
                        cJSON *objectItem = cJSON_CreateObject();
                        jsoncppToCJSON(element, objectItem);
                        cJSON_AddItemToArray(array, objectItem);
                    }
                    else
                    {
                        cJSON *value = cJSON_CreateString(element.asCString());
                        cJSON_AddItemToArray(array, value);
                    }
                }
            }
            else if (value.isString())
            {
                cJSON_AddStringToObject(cjsonObject, key.c_str(), value.asCString());
            }
            else if (value.isInt())
            {
                cJSON_AddNumberToObject(cjsonObject, key.c_str(), value.asInt());
            }
            else if (value.isDouble())
            {
                cJSON_AddNumberToObject(cjsonObject, key.c_str(), value.asDouble());
            }
            else if (value.isBool())
            {
                cJSON_AddBoolToObject(cjsonObject, key.c_str(), value.asBool());
            }
        }
    }

    cJSON *JsonFileHandler::generateCJSON(const Json::Value &jsonFile) const
    {
        cJSON *cjsonObject = cJSON_CreateObject();

        jsoncppToCJSON(jsonFile, cjsonObject);

        return cjsonObject;
    }

    void JsonFileHandler::printCJSON(const cJSON *cJsonObject)
    {
        char *cjsonStr = cJSON_Print(cJsonObject);
        std::cout << "CJSON representation:\n"
                  << cjsonStr << std::endl;
        cJSON_free(cjsonStr);
    }

};