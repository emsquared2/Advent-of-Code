#include <fstream>
#include <iostream>

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

bool next_position(int &direction, int &row, int &col, std::vector<std::vector<char>> &map,
                   std::vector<std::vector<bool>> &visited, int &distinct_positions)
{
    // Define movement deltas for directions: 0=Up, 1=Right, 2=Down, 3=Left
    const int dr[] = {-1, 0, 1, 0}; // Row change for each direction
    const int dc[] = {0, 1, 0, -1}; // Column change for each direction

    // Calculate next position
    int next_row = row + dr[direction];
    int next_col = col + dc[direction];

    // Check boundaries
    if (next_row < 0 || next_row >= map.size() || next_col < 0 || next_col >= map[0].size())
        return false;

    // Handle obstacle
    if (map[next_row][next_col] == '#')
    {
        direction = (direction + 1) % 4; // Rotate direction clockwise
        return true;                     // Continue with new direction
    }

    // Handle valid move
    if (!visited[next_row][next_col])
    {
        visited[next_row][next_col] = true;
        distinct_positions++;
    }

    // Update current position
    row = next_row;
    col = next_col;

    return true;
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

    int starting_row = -1;
    int starting_column = -1;

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
    }

    std::vector<std::vector<bool>> visited(total_rows, std::vector<bool>(total_columns, false));
    visited[starting_row][starting_column] = true;
    int distinct_positions = 1;
    int direction = 0;

    while (next_position(direction, starting_row, starting_column, map, visited, distinct_positions))
        ;

    std::cout << "The guard visited " << distinct_positions << " distinct positions before leaving the mapped area." << std::endl;

    return 0;
}