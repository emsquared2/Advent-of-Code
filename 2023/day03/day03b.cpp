#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>

bool isSymbol(char c)
{
    return !(std::isdigit(c) || c == '.');
}

bool inBounds(int r, int c, int rows, int cols)
{
    return r >= 0 && r < rows && c >= 0 && c < cols;
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

    int rows = grid.size();
    int cols = grid[0].size();

    // Map: position of '*' → list of adjacent part numbers
    std::map<std::pair<int, int>, std::vector<long long>> gearMap;

    for (int r = 0; r < rows; r++)
    {
        int c = 0;
        while (c < cols)
        {

            if (std::isdigit(grid[r][c]))
            {

                int start = c;
                long long value = 0;

                // Parse entire number
                while (c < cols && std::isdigit(grid[r][c]))
                {
                    value = value * 10 + (grid[r][c] - '0');
                    c++;
                }
                int end = c - 1;

                // Check adjacency to gear symbols '*'
                for (int rr = r - 1; rr <= r + 1; rr++)
                {
                    for (int cc = start - 1; cc <= end + 1; cc++)
                    {
                        if (!inBounds(rr, cc, rows, cols))
                            continue;

                        if (grid[rr][cc] == '*')
                        {
                            gearMap[{rr, cc}].push_back(value);
                        }
                    }
                }
            }
            else
            {
                c++;
            }
        }
    }

    long long sum = 0;

    for (auto &p : gearMap)
    {
        auto &nums = p.second;
        if (nums.size() == 2)
        {
            sum += nums[0] * nums[1];
        }
    }

    std::cout << "The sum of all of the gear ratios in the engine schematic is: " << sum << std::endl;
    return 0;
}
