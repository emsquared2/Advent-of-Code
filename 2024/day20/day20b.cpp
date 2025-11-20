#include <fstream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include <set>

using namespace std;

const int INF = INT_MAX;

// Directions in terms of (dx, dy)
vector<pair<int, int>> directions = {
    {-1, 0}, // up
    {0, 1},  // right
    {1, 0},  // down
    {0, -1}  // left
};

vector<string> grid;

// Define a custom hash function for pair<int, int>
namespace std
{
    template <>
    struct hash<pair<int, int>>
    {
        int operator()(const pair<int, int> &p) const
        {
            // Combine the hash of both elements of the pair
            return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
        }
    };
}

bool inbounds(int row, int col)
{
    return (row >= 0 && col >= 0 && row < grid.size() && col < grid[0].size());
}

pair<int, int> find_position(char c)
{
    for (int i = 0; i < grid.size(); ++i)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == c)
            {
                return make_pair(i, j);
            }
        }
    }
    return make_pair(-1, -1);
}

bool read_input(const string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        cerr << "Error: Could not open the file" << endl;
        return false;
    }

    string line;
    while (getline(file, line))
    {
        grid.push_back(line);
    }
    return true;
}

// Check if a position is within bounds and valid to move
bool can_move(const pair<int, int> &position, const unordered_map<pair<int, int>, int> &visited)
{
    int i = position.first, j = position.second;
    return (inbounds(i, j) && visited.find(position) == visited.end() && (grid[i][j] == 'S' || grid[i][j] == 'E' || grid[i][j] == '.'));
}

// Traverse the grid from start to end using BFS and record the number of steps taken to each position
unordered_map<pair<int, int>, int> traverse_grid(pair<int, int> start, pair<int, int> end)
{
    unordered_map<pair<int, int>, int> visited;
    visited[start] = 0;
    pair<int, int> current_position = start;
    int current_step = 0;

    while (current_position != end)
    {
        current_step += 1;
        int i = current_position.first, j = current_position.second;
        for (auto [di, dj] : directions)
        {
            pair<int, int> new_position = {i + di, j + dj};
            if (can_move(new_position, visited))
            {
                current_position = new_position;
                visited[current_position] = current_step;
                break;
            }
        }
    }
    return visited;
}

// Calculate the Manhattan distance between two coordinates
int manhattan_distance(const pair<int, int> &coord1, const pair<int, int> &coord2)
{
    return abs(coord1.first - coord2.first) + abs(coord1.second - coord2.second);
}

// Get nearby positions within a 20-unit range
set<pair<int, int>> get_cheat_endpoints(const pair<int, int> &coords, const set<pair<int, int>> &visited)
{
    set<pair<int, int>> nearby_positions;
    int i = coords.first, j = coords.second;

    for (int di = -20; di <= 20; ++di)
    {
        int dj_max = 20 - abs(di);
        for (int dj = -dj_max; dj <= dj_max; ++dj)
        {
            pair<int, int> neighbor = {i + di, j + dj};
            if (visited.find(neighbor) != visited.end())
            {
                nearby_positions.insert(neighbor);
            }
        }
    }

    return nearby_positions;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    if (!read_input(argv[1]))
        return 1;

    pair<int, int> start = find_position('S');
    pair<int, int> end = find_position('E');

    // Traverse the grid and get visited steps
    unordered_map<pair<int, int>, int> visited = traverse_grid(start, end);

    // Track visited positions and count positions that meet the condition
    set<pair<int, int>> visted_positions;
    for (const auto &[position, _] : visited)
    {
        visted_positions.insert(position);
    }

    int count = 0;
    for (const auto &coords : visited)
    {
        set<pair<int, int>> nearby_positions = get_cheat_endpoints(coords.first, visted_positions);
        for (const auto &other_coords : nearby_positions)
        {
            int distance = manhattan_distance(coords.first, other_coords);
            if (visited.at(other_coords) - visited.at(coords.first) - distance >= 100)
            {
                count++;
            }
        }
    }

    cout << count << endl;

    return 0;
}
