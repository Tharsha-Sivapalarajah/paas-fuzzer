#include <iostream>
#include <fstream>
#include <json/json.h>
#include <iomanip>
#include <cstdlib>
#include <ctime>

#include "Patch.h"

namespace driver
{
    Patch::Patch(std::vector<std::string> keys, double score) : keys(keys), score(score) {}

    std::vector<std::string> Patch::getKeys() const
    {
        return keys;
    }

    double Patch::getScore() const
    {
        return score;
    }

    bool Patch::setScore(double newScore)
    {
        score = newScore;
        return true;
    }
}
