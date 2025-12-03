#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

bool isSymbol(char c)
{
    return !(std::isdigit(c) || c == '.');
}

bool hasAdjacentSymbol(const std::vector<std::string> &grid, int r, int cStart, int cEnd)
{
    int rows = grid.size();
    int cols = grid[0].size();

    for (int rr = r - 1; rr <= r + 1; ++rr)
    {
        for (int cc = cStart - 1; cc <= cEnd + 1; ++cc)
        {
            if (rr == r && (cc >= cStart && cc <= cEnd))
                continue; // skip checking inside the number itself

            if (rr >= 0 && rr < rows && cc >= 0 && cc < cols)
            {
                if (isSymbol(grid[rr][cc]))
                {
                    return true;
                }
            }
        }
    }
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
    if (!file)
    {
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    std::vector<std::string> grid;
    std::string line;
    while (std::getline(file, line))
    {
        grid.push_back(line);
    }

    long long sum = 0;

    for (int r = 0; r < grid.size(); r++)
    {
        int c = 0;
        while (c < grid[r].size())
        {
            if (std::isdigit(grid[r][c]))
            {
                int start = c;
                std::string number;

                // collect full multi-digit number
                while (c < grid[r].size() && std::isdigit(grid[r][c]))
                {
                    number += grid[r][c];
                    c++;
                }

                int end = c - 1;

                // check adjacency
                if (hasAdjacentSymbol(grid, r, start, end))
                {
                    sum += std::stoll(number);
                }
            }
            else
            {
                c++;
            }
        }
    }

    std::cout << "the sum of all of the part numbers in the engine schematic is: " << sum << std::endl;

    return 0;
}
