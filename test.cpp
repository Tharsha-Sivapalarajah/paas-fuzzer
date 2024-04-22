#include <iostream>
#include <fstream>
#include <vector>

void writeIntArrayToFile(const std::vector<int> &arr, const std::string &filename)
{
    std::ofstream outfile(filename, std::ios::app); // Open file in append mode
    if (outfile.is_open())
    {
        for (int i : arr)
        {
            outfile << i << " ";
        }
        outfile << std::endl;
        outfile.close();
        std::cout << "Array appended to file successfully." << std::endl;
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

int main()
{
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    std::vector<int> arr2 = {6, 7, 8, 9, 10};
    std::string filename = "numbers.txt";

    // Write arr1 to file
    writeIntArrayToFile(arr1, filename);

    // Append arr2 to file
    writeIntArrayToFile(arr2, filename);

    return 0;
}
