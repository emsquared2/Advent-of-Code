#include <fstream>
#include <iostream>
#include <map>
#include <set>

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

void find_trailheads(int row, int col, int height, const std::vector<std::vector<int>> &map,
                     std::map<std::pair<int, int>, int> &trailhead_scores,
                     std::set<std::pair<int, int>> &visited_positions)
{
    int total_rows = map.size();
    int total_cols = map[0].size();

    if (row < 0 || row >= total_rows || col < 0 || col >= total_cols || map[row][col] != (height - 1))
        return;

    if (visited_positions.count({row, col}) > 0)
        return;

    visited_positions.insert({row, col});

    if (map[row][col] == 0)
    {
        trailhead_scores[{row, col}] += 1;
    }
    else
    {
        find_trailheads(row - 1, col, height - 1, map, trailhead_scores, visited_positions);
        find_trailheads(row, col + 1, height - 1, map, trailhead_scores, visited_positions);
        find_trailheads(row + 1, col, height - 1, map, trailhead_scores, visited_positions);
        find_trailheads(row, col - 1, height - 1, map, trailhead_scores, visited_positions);
    }
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

    std::map<std::pair<int, int>, int> trailhead_scores;

    for (int i = 0; i < map.size(); ++i)
    {
        for (int j = 0; j < map[0].size(); ++j)
        {
            if (map[i][j] == 9)
            {
                std::set<std::pair<int, int>> visited_positions;
                find_trailheads(i, j, 10, map, trailhead_scores, visited_positions);
            }
        }
    }

    int sum = 0;
    for (const auto &entry : trailhead_scores)
        sum += entry.second;

    std::cout << "The sum of the scores of all trailheads is " << sum << std::endl;

    return 0;
}
