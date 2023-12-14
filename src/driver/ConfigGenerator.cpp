#include <iostream>
#include <fstream>
#include <json/json.h>
#include <iomanip>
#include <filesystem>

int main()
{
    // Read JSON file
    std::ifstream input_file("../../data/input.json");
    if (!input_file.is_open())
    {
        std::cerr << "Error opening input JSON file." << std::endl;
        return 1;
    }

    Json::Value input_json;
    input_file >> input_json;
    input_file.close();

    // Iterate through JSON values and manipulate them (for example, add 1 to each integer value)
    for (auto &element : input_json)
    {
        if (element.isInt())
        {
            element = element.asInt() + 1;
        }
    }

    // Write manipulated JSON to a new file
    std::ofstream output_file("../../data/output.json");
    if (!output_file.is_open())
    {
        std::cerr << "Error opening output JSON file." << std::endl;
        return 1;
    }

    // Use setw from iomanip for formatting
    output_file << std::setw(4) << input_json << std::endl;
    output_file.close();

    std::cout << "JSON manipulation completed successfully." << std::endl;

    return 0;
}
