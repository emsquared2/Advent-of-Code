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
                auto [row_a, col_a] = positions[i];
                auto [row_b, col_b] = positions[j];

                // Calculate potential antinode locations
                int antinode1_row = row_a + row_a - row_b;
                int antinode1_col = col_a + col_a - col_b;
                int antinode2_row = row_b + row_b - row_a;
                int antinode2_col = col_b + col_b - col_a;

                // Check if within bounds and add to set of unique antinodes
                if (antinode1_row >= 0 && antinode1_row < total_rows &&
                    antinode1_col >= 0 && antinode1_col < total_columns)
                {
                    unique_antinodes.emplace(antinode1_row, antinode1_col);
                }
                if (antinode2_row >= 0 && antinode2_row < total_rows &&
                    antinode2_col >= 0 && antinode2_col < total_columns)
                {
                    unique_antinodes.emplace(antinode2_row, antinode2_col);
                }
            }
        }
    }

    // Count unique locations
    int total_unique_antinodes = unique_antinodes.size();

    std::cout << "There are " << total_unique_antinodes << " antinode locations!" << std::endl;

    return 0;
}
