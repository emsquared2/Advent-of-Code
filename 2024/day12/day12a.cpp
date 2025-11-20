#include <fstream>
#include <iostream>
#include <map>
#include <set>

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
        std::vector<char> row;
        for (const char c : line)
            row.push_back(c);
        map.push_back(row);
    }

    file.close();
    return true;
}

void find_region(int row, int col, char plant, const std::vector<std::vector<char>> &map,
                 std::set<std::pair<int, int>> &visited_positions,
                 std::map<std::string, std::vector<std::pair<int, int>>> &regions,
                 const std::string &key)
{
    int total_rows = map.size();
    int total_cols = map[0].size();

    // Base cases: Out of bounds or wrong plant character or already visited
    if (row < 0 || row >= total_rows || col < 0 || col >= total_cols || map[row][col] != plant)
        return;

    if (visited_positions.count({row, col}) > 0)
        return;

    visited_positions.insert({row, col});

    // Add current position to the region with the specified key
    regions[key].push_back({row, col});

    // Explore all 4 neighbors (up, right, down, left)
    find_region(row - 1, col, plant, map, visited_positions, regions, key);
    find_region(row, col + 1, plant, map, visited_positions, regions, key);
    find_region(row + 1, col, plant, map, visited_positions, regions, key);
    find_region(row, col - 1, plant, map, visited_positions, regions, key);
}

int fence_region(const std::vector<std::pair<int, int>> &region_positions, const std::vector<std::vector<char>> &map)
{
    int region_size = region_positions.size();
    int perimeter = 0;

    int total_rows = map.size();
    int total_cols = map[0].size();

    // Directions for 4 neighbors: up, right, down, left
    const std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    for (const auto &[row, col] : region_positions)
    {
        for (const auto &[dr, dc] : directions)
        {
            int new_row = row + dr;
            int new_col = col + dc;

            // Check if the neighbor is out of bounds or different
            if (new_row < 0 || new_row >= total_rows || new_col < 0 || new_col >= total_cols || map[new_row][new_col] != map[row][col])
            {
                ++perimeter;
            }
        }
    }

    return region_size * perimeter;
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
        return 1;

    std::map<std::string, std::vector<std::pair<int, int>>> regions;
    std::set<std::pair<int, int>> visited_positions;

    // Track suffixes for unique keys
    std::map<char, int> suffix_counters;

    for (int i = 0; i < map.size(); ++i)
    {
        for (int j = 0; j < map[0].size(); ++j)
        {
            char plant = map[i][j];

            // Skip already visited positions
            if (visited_positions.count({i, j}) > 0)
                continue;

            // Generate a new key for this region
            std::string key = std::string(1, plant);
            if (regions.find(key) != regions.end())
            {
                // If the key already exists, add a suffix
                ++suffix_counters[plant];
                key += "_" + std::to_string(suffix_counters[plant]);
            }
            else
            {
                suffix_counters[plant] = 1; // Initialize counter
            }

            // Start exploring the new region
            find_region(i, j, plant, map, visited_positions, regions, key);
        }
    }

    int total_cost = 0;
    for (const auto &[key, positions] : regions)
        total_cost += fence_region(positions, map);

    std::cout << "The total price of fencing all regions is " << total_cost << "." << std::endl;

    return 0;
}
