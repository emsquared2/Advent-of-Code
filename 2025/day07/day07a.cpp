#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <set>

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
    int splits = 0;

    std::set<std::pair<int, int>> visited;
    std::queue<std::pair<int, int>> q;

    q.push({sr, sc});
    while (!q.empty())
    {
        auto [r, c] = q.front();
        q.pop();

        // Skip if this state was already processed
        if (visited.count({r, c}))
            continue;
        visited.insert({r, c});

        // Beam exits the manifold when reaching last row
        if (r + 1 == rows)
            continue;

        // If the next cell downward is a splitter '^',
        // then this beam stops and produces two new beams
        if (grid[r + 1][c] == '^')
        {
            q.push({r + 1, c - 1}); // left branch
            q.push({r + 1, c + 1}); // right branch
            splits++;
        }
        else
        {
            // Otherwise, the beam continues straight downward
            q.push({r + 1, c});
        }
    }

    std::cout << "The beam will split " << splits << " times.\n";
    return 0;
}