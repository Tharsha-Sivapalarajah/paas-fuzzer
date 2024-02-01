#include "ConfigGenerator.h"

using namespace driver;

namespace driver
{
    bool isInteger(const std::string &s)
    {
        try
        {
            std::stoi(s);
            return true;
        }
        catch (const std::invalid_argument &e)
        {
            return false;
        }
        catch (const std::out_of_range &e)
        {
            return false;
        }
    }

    int createIntegerPatch(std::vector<std::string> &keys, std::vector<driver::Patch> &patches, int start, int end, int step)
    {
        if (step <= 0)
        {
            step = 1;
        }
        int val = start;
        while (val <= end)
        {
            Patch patch(keys, 0.0, std::to_string(val));
            patches.push_back(patch);
            val = val + step;
        }
        return start;
    }

    std::string createEnumPatch(std::vector<std::string> &keys, std::vector<driver::Patch> &patches, std::vector<std::string> &values)
    {
        int count = 0;
        for (const auto &value : values)
        {
            count++;
            Patch patch(keys, 0.0, value);
            patches.push_back(patch);
        }
        return patches[patches.size() - count].getValue();
    }

    std::string generateRandomString(std::string seperator)
    {
        const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        const std::size_t partLength = 3;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> charIndex(0, sizeof(charset) - 2);

        std::string randomString;

        for (int i = 0; i < 3; ++i)
        {
            for (std::size_t j = 0; j < partLength; ++j)
            {
                randomString += charset[charIndex(gen)];
            }

            if (i < 2)
            {
                randomString += seperator;
            }
        }

        return randomString;
    }

    std::string createStringPatch(std::vector<std::string> &keys, std::vector<driver::Patch> &patches, int numberOfPatches, std::string seperator)
    {
        if (numberOfPatches <= 0)
        {
            numberOfPatches = 1;
        }

        std::string output;
        for (size_t i = 0; i < numberOfPatches; i++)
        {
            std::string tempRandomString = generateRandomString(seperator);
            if (i == 0)
            {
                output = tempRandomString;
            }
            Patch patch(keys, 0.0, tempRandomString);
            patches.push_back(patch);
        }
        return output;
    }

    void ConfigGenerator::createPatches(cJSON *inputFile, std::vector<std::vector<std::string>> &patchableKeys, std::vector<driver::Patch> &patches, int verbose)
    {
        if (verbose >= 5)
        {
            printf("Patchable Keys\n------------------------------------------\n");
        }

        for (auto &keys : patchableKeys)
        {
            cJSON *currentItem = inputFile;
            for (size_t i = 0; i < keys.size() - 1; i++)
            {
                const auto key = keys[i];
                if (isInteger(key))
                {
                    currentItem = cJSON_GetArrayItem(currentItem, std::stoi(key) - 1);
                    // array
                }
                else
                {
                    // object
                    currentItem = cJSON_GetObjectItem(currentItem, key.c_str());
                }
            }

            char *patchType;
            cJSON *patchObject;
            if (isInteger(keys[keys.size() - 1]))
            {
                patchObject = cJSON_GetArrayItem(currentItem, std::stoi(keys[keys.size() - 1]) - 1);
                patchType = cJSON_Print(cJSON_GetObjectItem(patchObject, "type"));
            }
            else
            {
                patchObject = cJSON_GetObjectItem(currentItem, keys[keys.size() - 1].c_str());
                patchType = cJSON_Print(cJSON_GetObjectItem(patchObject, "type"));
            }

            if (strcmp(patchType, "\"int\"") == 0)
            {
                int startInt, endInt, stepInt, initialValue;
                startInt = std::stoi(cJSON_Print(cJSON_GetObjectItem(patchObject, "start")));
                endInt = std::stoi(cJSON_Print(cJSON_GetObjectItem(patchObject, "end")));
                stepInt = std::stoi(cJSON_Print(cJSON_GetObjectItem(patchObject, "step")));
                initialValue = std::stoi(cJSON_Print(cJSON_GetObjectItem(patchObject, "start")));
                cJSON_ReplaceItemInObject(currentItem, keys[keys.size() - 1].c_str(), cJSON_CreateNumber(createIntegerPatch(keys, patches, startInt, endInt, stepInt)));
            }
            else if (strcmp(patchType, "\"enum\"") == 0)
            {
                std::vector<std::string> enums = {};
                cJSON *enumValues = cJSON_GetObjectItem(patchObject, "values")->child;
                while (enumValues != NULL)
                {
                    enums.push_back(enumValues->valuestring);

                    enumValues = enumValues->next;
                }
                std::string initialValue = createEnumPatch(keys, patches, enums);
                // std::cout << initialValue << std::endl;
                cJSON_ReplaceItemInObject(currentItem, keys[keys.size() - 1].c_str(), cJSON_CreateString(initialValue.c_str()));
            }
            else if (strcmp(patchType, "\"string\"") == 0)
            {
                char *seperator = cJSON_Print(cJSON_GetObjectItem(patchObject, "seperator"));
                std::size_t length = std::strlen(seperator);
                if (length >= 2 && seperator[0] == '"' && seperator[length - 1] == '"')
                {
                    seperator++;
                    seperator[length - 2] = '\0';
                }
                cJSON_ReplaceItemInObject(currentItem, keys[keys.size() - 1].c_str(), cJSON_CreateString(createStringPatch(keys, patches, 1, seperator).c_str()));
            }
            else
            {
                exit(0);
            }

            if (verbose >= 5)
            {
                for (const auto &key : keys)
                {
                    std::cout << key << "/";
                }
                std::cout << std::endl;
            }
        }
        printf("------------------------------------------\n");
    }

}
