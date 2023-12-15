#include <iostream>
#include <fstream>
#include <json/json.h>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using std::string;

class JsonFileHandler
{
public:
    JsonFileHandler(const string &inputFilePath) : inputFilePath(inputFilePath) {}

    // read the json file in the specified inputFilePath location and store the data in the jsonData argument.
    bool readJsonFile(Json::Value &jsonData)
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

    bool createConfigFile(Json::Value configJsonData)
    {
        for (auto &element : configJsonData)
        {
        }
    }

    bool manipulateJsonData(Json::Value jsonData)
    {
        for (auto &element : jsonData)
        {
            // Check if the element is an array
            if (element.isArray())
            {
                // Iterate through the elements of the array
                for (const auto &arrayElement : element)
                {
                    // Check if the element in the array is a string
                    if (arrayElement.isString())
                    {
                        // Do something with the string element
                        std::string stringValue = arrayElement.asString();
                        std::cout << "String in array: " << stringValue << std::endl;
                    }
                    else
                    {
                        // Handle other types if needed
                        std::cout << "Element in array is not a string." << std::endl;
                    }
                }
            }
            else
            {
                // Handle other types or non-array elements if needed
                std::cout << "Element is not an array." << std::endl;
            }
        }

        return true;
    }

    bool writeJsonFile(const Json::Value &jsonData, const std::string &outputFilePath)
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

private:
    int generateRandomNumber(int minValue, int maxValue)
    {
        // Seed the random number generator with the current time
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        // Generate a random number between minValue and maxValue (inclusive)
        return std::rand() % (maxValue - minValue + 1) + minValue;
    }

    string inputFilePath;
};