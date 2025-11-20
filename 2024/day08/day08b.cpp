#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

bool read_input_to_2d_vector(const std::string &filepath, std::vector<std::vector<char>> &map)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<char> row(line.begin(), line.end());
        map.push_back(row);
    }

    file.close();
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::vector<std::vector<char>> map;

    if (!read_input_to_2d_vector(argv[1], map))
    {
        return 1;
    }

    std::unordered_map<char, std::vector<std::pair<int, int>>> antenna_positions;

    int total_rows = map.size();
    int total_columns = map[0].size();

    // Collect antenna positions by frequency
    for (int i = 0; i < total_rows; ++i)
    {
        for (int j = 0; j < total_columns; ++j)
        {
            if (map[i][j] != '.')
            {
                antenna_positions[map[i][j]].emplace_back(i, j);
            }
        }
    }

    // Set to track unique antinode locations
    std::set<std::pair<int, int>> unique_antinodes;

    for (const auto &[key, positions] : antenna_positions)
    {
        for (int i = 0; i < positions.size(); ++i)
        {
            for (int j = i + 1; j < positions.size(); ++j)
            {
                auto [x1, y1] = positions[i];
                auto [x2, y2] = positions[j];

                int dx = x2 - x1;
                int dy = y2 - y1;

                int x = x1;
                int y = y1;

                int z = x2;
                int w = y2;

                do
                {

                    unique_antinodes.emplace(x, y);
                    map[x][y] = '#';

                    x += dx;
                    y += dy;
                } while (x >= 0 && x < total_rows && y >= 0 && y < total_columns);

                do
                {
                    unique_antinodes.emplace(z, w);
                    map[z][w] = '#';

                    z -= dx;
                    w -= dy;
                } while (z >= 0 && z < total_rows && w >= 0 && w < total_columns);
            }
        }
    }

    // Count unique locations
    int total_unique_antinodes = unique_antinodes.size();
    std::cout << "There are " << total_unique_antinodes << " antinode locations!" << std::endl;

    return 0;
}
