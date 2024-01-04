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
        std::string getPatchPathString() const;
        std::string getPatchValue() const;

        // setters
        bool setScore(double newScore);

    private:
        bool isInteger(const std::string &s) const;

        // members
        double score;
        std::string value;
        std::vector<std::string> keys;
    };
}