#pragma once

#include <json/json.h>
#include <iostream>
#include <regex>
#include <random>
#include <fstream>
#include <iomanip>
#include <filesystem>

#include "Patch.h"
#include "JsonFileHandler.h"

extern "C"
{
#include <external/cJSON.h>
#include <model/object.h>
}

namespace driver
{
    class ConfigGenerator
    {
    public:
        void createPatches(cJSON *inputFile, std::vector<std::vector<std::string>> &patchableKeys, std::vector<driver::Patch> &patches, int verbose);
    };

}