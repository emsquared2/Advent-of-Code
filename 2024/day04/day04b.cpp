#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

// Function to check if two "MAS" form an X shape
bool check_XMAS(const std::vector<std::string> &letters, int row, int col, std::regex &pattern)
{
    int rows = letters.size();
    int cols = letters[0].size();

    // Ensure the indices are within bounds
    if (row - 1 < 0 || row + 1 >= rows || col - 1 < 0 || col + 1 >= cols)
    {
        return false;
    }

    std::string a = std::string(1, letters[row - 1][col - 1]) + std::string(1, letters[row][col]) + std::string(1, letters[row + 1][col + 1]);
    std::string b = std::string(1, letters[row + 1][col - 1]) + std::string(1, letters[row][col]) + std::string(1, letters[row - 1][col + 1]);

    if ((a == "MAS" || a == "SAM") && (b == "MAS" || b == "SAM"))
        return true;

    return false;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return 1;
    }

    std::vector<std::string> letters;
    std::regex pattern(R"(MAS)");
    std::string line;

    int occurrences = 0;

    while (std::getline(file, line))
    {
        letters.push_back(line);
    }

    file.close();

    int total_rows = letters.size();
    int total_columns = letters[0].size();

    // Check for X-MAS patterns in the grid
    for (int row = 1; row < total_rows - 1; ++row)
    {
        for (int col = 1; col < total_columns - 1; ++col)
        {
            // Check if the X-MAS pattern is present centered at (row, col)
            if (check_XMAS(letters, row, col, pattern))
            {
                occurrences++;
            }
        }
    }

    std::cout << "X-MAS appeared " << occurrences << " times!" << std::endl;

    return 0;
}
