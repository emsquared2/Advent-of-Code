#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <set>

long long dfs(const std::vector<std::string> &grid, int r, int c, std::vector<std::vector<long long>> &memo)
{
    // Out of bounds → no beam
    if (c < 0 || c >= grid[0].size())
        return 0;

    // If at the last row → count this endpoint as 1
    if (r + 1 == grid.size())
        return 1;

    // Return memoized result if already computed
    if (memo[r][c] != -1)
        return memo[r][c];

    long long result = 0;

    // Splitter case
    if (grid[r + 1][c] == '^')
    {
        result = dfs(grid, r + 1, c - 1, memo) + dfs(grid, r + 1, c + 1, memo);
    }
    else
    {
        // Straight downward movement
        result = dfs(grid, r + 1, c, memo);
    }

    return memo[r][c] = result;
}
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

    // Read the grid (each line is a row)
    std::vector<std::string> grid;
    std::string line;
    while (std::getline(file, line))
        grid.push_back(line);

    int rows = grid.size();
    int cols = grid[0].size();

    // Locate the starting position 'S' in the grid
    int sr = -1, sc = -1;
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            if (grid[r][c] == 'S')
            {
                sr = r;
                sc = c;
                break;
            }
        }
    }
    // Memo table initialized to -1
    std::vector<std::vector<long long>> memo(rows, std::vector<long long>(cols, -1));

    long long timelines = dfs(grid, sr, sc, memo);

    std::cout << "A single tachyon particle would end up with " << timelines << " different timelines.\n";
    return 0;
}