#include "Patch.h"

namespace driver
{
    Patch::Patch(std::vector<std::string> keys, double score, std::string value) : keys(keys), score(score), value(value) {}

    std::vector<std::string> Patch::getKeys() const
    {
        return keys;
    }
    /*
{
    "spec":
        {
            "replicas":3
        }
}

{\"spec\":{\"replicas\":3}}
*/

    bool isInteger(const std::string& s) {
        for (char c : s) {
            if (!std::isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    std::string Patch::getStringFromPatch() const {
        std::vector<std::string> keys = this->getKeys();
        std::string finalValue = keys.back();
        keys.pop_back();
        for (const std::string &pathKey : keys) {
            if (isInteger(pathKey)) {
                pathKey+"\":";
            } else {
            }
            std::cout << "{\""+pathKey+"\":" << std::endl;
        }
        return "a";
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

}
