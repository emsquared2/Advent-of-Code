#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::pair<int, int>> directions = {
    {-1, 0},  // Up
    {1, 0},   // Down
    {0, -1},  // Left
    {0, 1},   // Right
    {-1, -1}, // Up-Left
    {-1, 1},  // Up-Right
    {1, -1},  // Down-Left
    {1, 1}    // Down-Right
};

bool isAccessed(const std::vector<std::vector<char>> &grid, int r, int c)
{
    int rows = grid.size();
    int cols = grid[0].size();

    int neighbors = 0;

    for (const auto &[dr, dc] : directions)
    {
        int nr = r + dr;
        int nc = c + dc;

        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] == '@')
            ++neighbors;
    }
    return neighbors < 4;
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

    std::vector<std::vector<char>> grid;
    std::string line;
    while (std::getline(file, line))
    {
        grid.emplace_back(line.begin(), line.end());
    }

    int rolls = 0;

    int rows = grid.size();
    int cols = grid[0].size();

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            if (grid[r][c] == '@' && isAccessed(grid, r, c))
                ++rolls;
        }
    }

    std::cout << "Total accessed rolls of paper: " << rolls << std::endl;

    return 0;
}