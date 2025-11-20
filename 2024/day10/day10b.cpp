#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

bool read_input_to_2d_vector(const std::string &filepath, std::vector<std::vector<int>> &map)
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
        std::vector<int> row;
        for (const char c : line)
            row.push_back(c - '0');
        map.push_back(row);
    }

    file.close();
    return true;
}

int count_distinct_trails(int row, int col, int height, const std::vector<std::vector<int>> &map, std::set<std::pair<int, int>> &visited_positions)
{
    int total_rows = map.size();
    int total_cols = map[0].size();

    if (row < 0 || row >= total_rows || col < 0 || col >= total_cols || map[row][col] != height)
        return 0;

    if (visited_positions.count({row, col}) > 0)
        return 0;

    if (height == 9)
        return 1; // Reached the end of a valid trail

    visited_positions.insert({row, col});

    int trails = 0;
    trails += count_distinct_trails(row - 1, col, height + 1, map, visited_positions);
    trails += count_distinct_trails(row, col + 1, height + 1, map, visited_positions);
    trails += count_distinct_trails(row + 1, col, height + 1, map, visited_positions);
    trails += count_distinct_trails(row, col - 1, height + 1, map, visited_positions);

    visited_positions.erase({row, col}); // Backtrack for other trails

    return trails;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> map;

    if (!read_input_to_2d_vector(argv[1], map))
        return 1;

    std::map<std::pair<int, int>, int> trailhead_ratings;

    for (int i = 0; i < map.size(); ++i)
    {
        for (int j = 0; j < map[0].size(); ++j)
        {
            if (map[i][j] == 0)
            {
                std::set<std::pair<int, int>> visited_positions;
                trailhead_ratings[{i, j}] = count_distinct_trails(i, j, 0, map, visited_positions);
            }
        }
    }

    int sum = 0;
    for (const auto &entry : trailhead_ratings)
        sum += entry.second;

    std::cout << "The sum of the ratings of all trailheads is " << sum << std::endl;

    return 0;
}
