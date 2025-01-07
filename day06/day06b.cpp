#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <tuple>

// Read input map from a file into a 2D vector
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

// Simulate the guard's movement and check if a loop occurs
bool causes_loop(int start_row, int start_col, const std::vector<std::vector<char>> &map)
{
    // Direction vectors: 0=Up, 1=Right, 2=Down, 3=Left
    const int dr[] = {-1, 0, 1, 0};
    const int dc[] = {0, 1, 0, -1};

    int row = start_row, col = start_col, direction = 0;
    std::set<std::tuple<int, int, int>> visited_states;

    while (true)
    {
        // Save current state
        auto current_state = std::make_tuple(row, col, direction);

        // Detect loop
        if (visited_states.count(current_state))
            return true;

        visited_states.insert(current_state);

        // Calculate next position
        int next_row = row + dr[direction];
        int next_col = col + dc[direction];

        // Check boundaries
        if (next_row < 0 || next_row >= map.size() || next_col < 0 || next_col >= map[0].size())
            break;

        // Obstacle handling
        if (map[next_row][next_col] == '#')
        {
            direction = (direction + 1) % 4; // Rotate clockwise
        }
        else
        {
            // Move to the next position
            row = next_row;
            col = next_col;
        }
    }

    return false; // No loop detected
}

int main(int argc, char *argv[])
{
    std::vector<std::vector<char>> map;
    if (!read_input_to_2d_vector(argv[1], map))
    {
        return 1;
    }

    int total_rows = map.size();
    int total_columns = map[0].size();

    int starting_row = -1, starting_column = -1;

    // Locate the starting position of the guard
    for (int i = 0; i < total_rows; ++i)
    {
        for (int j = 0; j < total_columns; ++j)
        {
            if (map[i][j] == '^')
            {
                starting_row = i;
                starting_column = j;
                break;
            }
        }
        if (starting_row != -1)
            break;
    }

    int loop_count = 0;

    // Check every position on the map for potential obstruction
    for (int i = 0; i < total_rows; ++i)
    {
        for (int j = 0; j < total_columns; ++j)
        {
            if (map[i][j] == '.' && !(i == starting_row && j == starting_column))
            {
                // Temporarily modify the map
                map[i][j] = '#';

                // Check if the modification causes a loop
                if (causes_loop(starting_row, starting_column, map))
                    loop_count++;

                // Restore the map
                map[i][j] = '.';
            }
        }
    }

    std::cout << "There are " << loop_count << " positions that can cause a loop if changed to '#'." << std::endl;

    return 0;
}
