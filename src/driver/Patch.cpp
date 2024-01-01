#include "Patch.h"

namespace driver
{
    Patch::Patch(std::vector<std::string> keys, double score, std::string value) : keys(keys), score(score), value(value) {}

    std::vector<std::string> Patch::getKeys() const
    {
        return keys;
    }

    double Patch::getScore() const
    {
        return score;
    }

    std::string Patch::getValue() const
    {
        return value;
    }

    bool Patch::setScore(double newScore)
    {
        score = newScore;
        return true;
    }

    std::string Patch ::getStringFromPatches(std::vector<Patch>)
    {
    }
}
