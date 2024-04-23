#include "JsonFileHandler.h"

namespace driver
{

    bool JsonFileHandler::bugReportDirectoryCreated = false;

    char *JsonFileHandler::readJSONFile(const char *filename) const
    {
        // create a readable FILE obejct to read json file
        FILE *file = fopen(filename, "r");
        if (file == NULL)
        {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *json_data = (char *)malloc(file_size + 1);
        fread(json_data, 1, file_size, file);
        fclose(file);

        json_data[file_size] = '\0'; // Null-terminate the string
        return json_data;
    }

    cJSON *JsonFileHandler::createJSONObject(const char *filename) const
    {
        // read and get the char of the json input file
        char *json_data = readJSONFile(filename);
        // parse the input char to cJSON object
        cJSON *root = cJSON_Parse(json_data);

        // error handling if parsing failed
        if (root == NULL)
        {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL)
            {
                fprintf(stderr, "Error before: %s\n", error_ptr);
            }
            exit(EXIT_FAILURE);
        }
        return root;
    }

    bool JsonFileHandler::compareCJSONObjects(cJSON *obj1, cJSON *obj2, std::vector<std::string> &keys)
    {
        cJSON *currentItem_obj1 = obj1->child;
        while (currentItem_obj1 != NULL)
        {
            if (currentItem_obj1->type == cJSON_String || currentItem_obj1->type == cJSON_Number)
            {
                keys.push_back(currentItem_obj1->string);
                cJSON *temp = obj2;
                for (size_t i = 0; i < keys.size(); i++)
                {
                    auto &key = keys[i];
                    if (isInteger(key))
                    {
                        temp = cJSON_GetArrayItem(temp, std::stoi(key));
                    }
                    else
                    {
                        temp = cJSON_GetObjectItem(temp, const_cast<char *>(key.c_str()));
                    }
                }

                if (currentItem_obj1->type == cJSON_String)
                {
                    if (strcmp(currentItem_obj1->valuestring, temp->valuestring) != 0)
                    {
                        return false;
                    }
                }
                else
                {
                    if (currentItem_obj1->valueint != temp->valueint)
                    {
                        return false;
                    }
                }
                keys.pop_back();
            }
            else if (currentItem_obj1->type == cJSON_Object)
            {
                keys.push_back(currentItem_obj1->string);
                if (!compareCJSONObjects(currentItem_obj1, obj2, keys))
                    return false;
                keys.pop_back();
            }
            else if (currentItem_obj1->type == cJSON_Array)
            {
                int count = 0;
                cJSON *currentArrayElement = currentItem_obj1->child;
                while (currentArrayElement != NULL)
                {
                    keys.push_back(currentItem_obj1->string);
                    keys.push_back(std::to_string(count));
                    if (currentArrayElement->type == cJSON_Object)
                    {
                        if (!compareCJSONObjects(currentArrayElement, obj2, keys))
                            return false;
                        keys.pop_back();
                        keys.pop_back();
                    }
                    else
                    {
                        cJSON *temp = obj2;
                        for (size_t i = 0; i < keys.size(); i++)
                        {
                            auto &key = keys[i];
                            if (isInteger(key))
                            {
                                temp = cJSON_GetArrayItem(temp, std::stoi(key));
                            }
                            else
                            {
                                temp = cJSON_GetObjectItem(temp, const_cast<char *>(key.c_str()));
                            }
                        }

                        if (temp->type == cJSON_String)
                        {
                            if (strcmp(temp->valuestring, currentArrayElement->valuestring) != 0)
                            {
                                return false;
                            }
                        }
                        else if (temp->type == cJSON_Number)
                        {
                            if (temp->valueint != currentArrayElement->valueint)
                            {
                                return false;
                            }
                        }
                        keys.pop_back();
                        keys.pop_back();
                    }
                    currentArrayElement = currentArrayElement->next;
                    count++;
                }
            }
            // Move to the next item
            currentItem_obj1 = currentItem_obj1->next;
        }
        return true;
    }

    std::string JsonFileHandler::cJSONToString(cJSON *json)
    {
        char *jsonString = cJSON_Print(json);
        std::string result(jsonString);
        free(jsonString);
        return result;
    }

    cJSON *JsonFileHandler::createConfigFile(const cJSON *inputJson, std::vector<std::string> keys, std::vector<std::vector<std::string>> &patchableKeys)
    {
        cJSON *currentItem = inputJson->child;
        while (currentItem != NULL)
        {
            if (currentItem->type == cJSON_Object)
            {
                keys.push_back(currentItem->string);
                // Recursively iterate through nested objects
                createConfigFile(currentItem, keys, patchableKeys);
                keys.pop_back();
            }
            else if (currentItem->type == cJSON_String)
            {
                // Check if the key is "key" and the value is "patch"
                if (strcmp(currentItem->string, "operation") == 0 && strcmp(currentItem->valuestring, "patch") == 0)
                {
                    patchableKeys.push_back(keys);
                }
            }
            else if (currentItem->type == cJSON_Array)
            {
                int count = 0;
                cJSON *currentArrayElement = currentItem->child;
                while (currentArrayElement != NULL)
                {
                    count++;
                    if (currentArrayElement->type == cJSON_Object)
                    {
                        keys.push_back(currentItem->string);
                        keys.push_back(std::to_string(count));
                        createConfigFile(currentArrayElement, keys, patchableKeys);
                        keys.pop_back();
                        keys.pop_back();
                    }
                    else
                    {
                        keys.push_back(currentItem->string);
                        createConfigFile(currentArrayElement, keys, patchableKeys);
                        keys.pop_back();
                    }
                    currentArrayElement = currentArrayElement->next;
                }
            }

            // Move to the next item
            currentItem = currentItem->next;
        }
        return currentItem;
    }

    // bool JsonFileHandler::isInteger(const std::string &s) const
    // {
    //     try
    //     {
    //         std::stoi(s);
    //         return true;
    //     }
    //     catch (const std::invalid_argument &e)
    //     {
    //         return false;
    //     }
    //     catch (const std::out_of_range &e)
    //     {
    //         return false;
    //     }
    // }

    bool JsonFileHandler::isInteger(const std::string &s) const
    {
        try
        {
            size_t pos;
            int value = std::stoi(s, &pos);
            return pos == s.length();
        }
        catch (const std::invalid_argument &)
        {
            return false;
        }
        catch (const std::out_of_range &)
        {
            return false;
        }
    }

    void JsonFileHandler::modifyPatch(cJSON *jsonData, driver::Patch patch) const
    {
        cJSON *currentItem = jsonData;
        std::vector<std::string> patchKeys = patch.getKeys();

        for (size_t i = 0; i < patchKeys.size() - 1; i++)
        {
            const auto key = patchKeys[i];

            if (isInteger(key))
            {
                // array
                currentItem = cJSON_GetArrayItem(currentItem, std::stoi(key) - 1);
            }
            else
            {
                // object
                currentItem = cJSON_GetObjectItem(currentItem, key.c_str());
            }
        }

        if (isInteger(patch.getValue()))
        {
            cJSON_ReplaceItemInObject(currentItem, patchKeys[patchKeys.size() - 1].c_str(), cJSON_CreateNumber(std::stoi(patch.getValue())));
            // std::cout << "Integer patch: " << patch.getValue() << std::endl;
        }
        else
        {
            cJSON_ReplaceItemInObject(currentItem, patchKeys[patchKeys.size() - 1].c_str(), cJSON_CreateString(patch.getValue().c_str()));
            // std::cout << "String patch: " << patch.getValue() << std::endl;
        }
    }

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

    std::string JsonFileHandler::generateRandomFileName() const
    {
        // Get the current time in milliseconds
        auto currentTime = std::chrono::system_clock::now();
        auto timeSinceEpoch = currentTime.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceEpoch).count();

        // Use a random number for additional uniqueness
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(0, 9999);
        int randomSuffix = distribution(gen);

        // Combine timestamp and random number to create a unique filename
        std::string fileName = "file_" + std::to_string(milliseconds) + "_" + std::to_string(randomSuffix);

        return fileName;
    }

    bool JsonFileHandler::isPatchIncluded(cJSON *jsonData, Patch &patch, int verbose) const
    {
        cJSON *currentJson = jsonData;
        std::vector<std::string> patchKeys = patch.getKeys();
        for (size_t i = 0; i < patchKeys.size(); i++)
        {
            std::string key = patchKeys[i];
            if (isInteger(key))
            {
                currentJson = cJSON_GetArrayItem(currentJson, std::stoi(key) - 1);
            }
            else
            {
                currentJson = cJSON_GetObjectItem(currentJson, key.c_str());
            }
        }

        if (isInteger(patch.getValue()))
        {
            if (currentJson->valueint)
            {
                if (std::stoi(patch.getValue()) == currentJson->valueint)
                {
                    return true;
                }
            }
            else if (currentJson->valuestring)
            {
                if (strcmp(patch.getValue().c_str(), currentJson->valuestring) == 0)
                {
                    return true;
                }
            }
            return false;
        }
        else
        {
            if (currentJson->valueint)
            {
                if (std::stoi(patch.getValue()) == currentJson->valueint)
                {
                    return true;
                }
            }
            else if (currentJson->valuestring)
            {
                if (strcmp(patch.getValue().c_str(), currentJson->valuestring) == 0)
                {
                    return true;
                }
            }
            return false;
        }
    }

    void createBugReportDirectory(std::string directoryPath, bool &bugReportDirectoryCreated, int verbose)
    {
        if (bugReportDirectoryCreated)
        {
            // bug directory already created
            if (!fs::exists(directoryPath)) // check if the directory actually doesn't exists
            {
                std::cout << "Something went wrong in the bug report directory: " << directoryPath << std::endl;
                bugReportDirectoryCreated = false;
                exit(0);
            }
            else
            {
                // directory actually exists as expected

                bugReportDirectoryCreated = true;
            }
        }
        else
        {
            // bug directory is not created yet
            // directory can still exists, if so, should delete the directory
            if (!fs::exists(directoryPath))
            {
                // directory actually doesn't exist as expected
                fs::create_directory(directoryPath);
                if (verbose >= 3)
                    std::cout << "Directory '" << directoryPath << "' created." << std::endl;
            }
            else
            {
                // directory exists even though it should not
                if (verbose >= 3)
                    std::cout << "Directory '" << directoryPath << "' already exists." << std::endl;
                fs::remove_all(directoryPath);
                if (verbose >= 3)
                    std::cout << "Directory '" << directoryPath << "' and its content successfully removed." << std::endl;
                fs::create_directory(directoryPath);
                if (verbose >= 3)
                    std::cout << "Directory '" << directoryPath << "' created." << std::endl;

                bugReportDirectoryCreated = true;
            }
        }
    }

    bool JsonFileHandler::reportBug(cJSON *previousConfigFile, cJSON *currentConfigFile, std::vector<driver::Patch> &patches, driver::Patch &currentPatch, int verbose)
    {
        try
        {
            std::string directoryPath = "./bugReport";
            createBugReportDirectory(directoryPath, bugReportDirectoryCreated, verbose);
            std::string randomFilePrefix = this->generateRandomFileName();
            directoryPath = directoryPath + "/" + randomFilePrefix;
            bool temp = false;
            createBugReportDirectory(directoryPath, temp, verbose);

            std::string previousConfig = directoryPath + "/" + randomFilePrefix + "_previous.json";
            std::string currentConfig = directoryPath + "/" + randomFilePrefix + "_current.json";
            std::string permutationList = directoryPath + "/" + randomFilePrefix + "_permutation.txt";

            // Write the JSON string to a file
            std::ofstream outputFilePrevious(previousConfig);
            if (outputFilePrevious.is_open())
            {
                outputFilePrevious << cJSON_Print(previousConfigFile);
                outputFilePrevious.close();
                if (verbose >= 3)
                    std::cout << "JSON object saved to " << previousConfig << std::endl;
            }
            else
            {
                if (verbose >= 3)
                    std::cerr << "Unable to open file for writing previous config." << std::endl;
                return false;
            }

            std::ofstream outputFileCurrent(currentConfig);
            if (outputFileCurrent.is_open())
            {
                outputFileCurrent << cJSON_Print(currentConfigFile);
                outputFileCurrent.close();
                if (verbose >= 3)
                    std::cout << "JSON object saved to " << currentConfig << std::endl;
            }
            else
            {
                std::cerr << "Unable to open file for writing current config." << std::endl;
                return false;
            }

            driver::Evaluator evaluator;
            evaluator.writeToFile(patches, permutationList);

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error creating bug report: " << e.what() << std::endl;
            return false;
        }
    }

};