#pragma once

#include <iostream>
#include <json/json.h>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

namespace driver
{
    class Patch
    {
    public:
        Patch(std::vector<std::string> keys, double score, std::string value);

        // getters
        std::vector<std::string> getKeys() const;
        double getScore() const;
        std::string getValue() const;
        std::string getStringFromPatch() const;

        // setters
        bool setScore(double newScore);

        std::vector<std::string> keys;

    private:
        // members
        double score;
        std::string value;
    };
}