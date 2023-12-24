#pragma once

#include <iostream>
#include <json/json.h>

namespace driver
{
    class Patch
    {
    public:
        Patch(std::vector<std::string> keys, double score);

        // getters
        std::vector<std::string> getKeys() const;
        double getScore() const;

        // setters
        bool setScore(double newScore);

    private:
        // members
        std::vector<std::string> keys;
        double score;
    };
}