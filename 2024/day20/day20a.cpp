#include <fstream>
#include <vector>
#include <iostream>
#include <queue>
#include <climits>
#include <set>
#include <tuple>
#include <utility>
#include <unordered_map>

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

// Traverse the grid from start to end using a greedy approach
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

// Count the special positions where a move sequence is valid and the time difference is significant
int count_special_positions(const unordered_map<pair<int, int>, int> &visited)
{
    int count = 0;
    for (const auto &[position, step] : visited)
    {
        int i = position.first, j = position.second;
        for (auto [di, dj] : directions)
        {
            pair<int, int> first_neighbor = {i + di, j + dj};
            pair<int, int> second_neighbor = {i + 2 * di, j + 2 * dj};

            if (visited.find(first_neighbor) == visited.end() && visited.find(second_neighbor) != visited.end() &&
                visited.at(second_neighbor) - step >= 102)
            {
                count++;
            }
        }
    }
    return count;
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

    pair<int, int> start_pos = find_position('S');
    pair<int, int> end_pos = find_position('E');

    if (start_pos == make_pair(-1, -1) || end_pos == make_pair(-1, -1))
    {
        cerr << "Error: Start or End position not found in grid" << endl;
        return 1;
    }

    // Traverse the grid to count special positions
    unordered_map<pair<int, int>, int> visited = traverse_grid(start_pos, end_pos);
    int result = count_special_positions(visited);

    cout << result << endl;

    return 0;
}
