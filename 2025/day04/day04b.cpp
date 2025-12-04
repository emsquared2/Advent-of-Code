#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

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

    int rows = grid.size();
    int cols = grid[0].size();

    // Step 1: Compute adjacency counts
    std::vector<std::vector<int>> adj(rows, std::vector<int>(cols, 0));

    auto isInBounds = [&](int r, int c)
    {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    };

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            if (grid[r][c] == '@')
            {
                for (auto [dr, dc] : directions)
                {
                    int nr = r + dr;
                    int nc = c + dc;
                    if (isInBounds(nr, nc) && grid[nr][nc] == '@')
                        adj[r][c]++;
                }
            }
        }
    }

    // Step 2: Initialize queue with all accessible rolls (<4 neighbors)
    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<bool>> removed(rows, std::vector<bool>(cols, false));

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            if (grid[r][c] == '@' && adj[r][c] < 4)
            {
                q.emplace(r, c);
                removed[r][c] = true;
            }
        }
    }

    // Step 3: BFS-like peeling process
    int total_removed = 0;
    while (!q.empty())
    {
        auto [r, c] = q.front();
        q.pop();
        ++total_removed;

        for (auto [dr, dc] : directions)
        {
            int nr = r + dr;
            int nc = c + dc;
            if (!isInBounds(nr, nc) || grid[nr][nc] != '@' || removed[nr][nc])
                continue;

            // This neighbor loses one adjacent roll
            --adj[nr][nc];

            if (adj[nr][nc] < 4)
            {
                q.emplace(nr, nc);
                removed[nr][nc] = true;
            }
        }
    }

    std::cout << "Total removed rolls: " << total_removed << std::endl;

    return 0;
}