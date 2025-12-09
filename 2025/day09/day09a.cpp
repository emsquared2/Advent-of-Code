#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    std::vector<std::pair<int, int>> tiles;

    int r, c;
    char comma;
    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        if (ss >> c >> comma >> r)
        {
            tiles.emplace_back(r, c);
        }
    }

    int n = tiles.size();
    long long maxArea = 0;

    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            int r1 = tiles[i].first;
            int c1 = tiles[i].second;
            int r2 = tiles[j].first;
            int c2 = tiles[j].second;

            long long height = std::abs(r2 - r1) + 1;
            long long width = std::abs(c2 - c1) + 1;

            maxArea = std::max(maxArea, height * width);
        }
    }

    std::cout << "Max area: " << maxArea << "\n";

    return 0;
}