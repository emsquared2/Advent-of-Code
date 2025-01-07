#include <fstream>
#include <vector>
#include <iostream>
#include <string>

std::string moves;
std::vector<std::vector<char>> map;

void print_map()
{
    for (const auto &row : map)
    {
        for (const auto &cell : row)
            std::cout << cell;
        std::cout << std::endl;
    }
    std::cout << "---------" << std::endl;
}

bool read_input(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return false;
    }

    std::string line;
    bool reading_moves = false;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            reading_moves = true;
            continue;
        }

        if (!reading_moves)
        {
            std::vector<char> row(line.begin(), line.end());
            map.push_back(row);
        }
        else
        {
            moves += line;
        }
    }

    file.close();
    return true;
}

bool inbounds(int row, int col)
{
    return (row >= 0 && col >= 0 && row < map.size() && col < map[0].size());
}

std::pair<int, int> find_robot()
{
    for (int i = 0; i < map.size(); ++i)
    {
        for (int j = 0; j < map[i].size(); ++j)
        {
            if (map[i][j] == '@')
                return {i, j};
        }
    }
    return {0, 0};
}

bool push_boxes(int x, int y, int dx, int dy)
{
    // Base case: if out of bounds or wall, abort the push
    if (!inbounds(x, y) || map[x][y] == '#')
        return false;

    // If it's an empty space, we can push all preceding boxes
    if (map[x][y] == '.')
    {
        map[x][y] = 'O';
        return true;
    }

    // If it's another box, attempt to push it further
    if (map[x][y] == 'O')
    {
        if (push_boxes(x + dx, y + dy, dx, dy))
        {
            // If pushing the next box was successful, move this box
            map[x + dx][y + dy] = 'O';
            return true;
        }
    }

    // If neither condition is met, we can't push
    return false;
}

void move(const char c, int &robot_x, int &robot_y)
{
    int dx = 0, dy = 0;
    if (c == '^')
        dx = -1;
    else if (c == 'v')
        dx = 1;
    else if (c == '<')
        dy = -1;
    else if (c == '>')
        dy = 1;

    int new_x = robot_x + dx;
    int new_y = robot_y + dy;

    if (!inbounds(new_x, new_y) || map[new_x][new_y] == '#')
        return;

    if (map[new_x][new_y] == '.')
    {
        // Move robot to empty space
        map[robot_x][robot_y] = '.';
        map[new_x][new_y] = '@';
        robot_x = new_x;
        robot_y = new_y;
    }
    else if (map[new_x][new_y] == 'O')
    {
        // Attempt to push boxes
        if (push_boxes(new_x + dx, new_y + dy, dx, dy))
        {
            // If successful, move the robot
            map[robot_x][robot_y] = '.';
            robot_x = new_x;
            robot_y = new_y;
            map[robot_x][robot_y] = '@';
        }
    }
}

int sum_GPS_coordinates()
{
    int sum = 0;
    for (int i = 0; i < map.size(); ++i)
    {
        for (int j = 0; j < map[i].size(); ++j)
        {
            if (map[i][j] == 'O')
                sum += i * 100 + j;
        }
    }
    return sum;
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

    int robot_x, robot_y;
    std::pair<int, int> initial_location = find_robot();
    robot_x = initial_location.first;
    robot_y = initial_location.second;

    for (const char c : moves)
    {
        move(c, robot_x, robot_y);
    }

    std::cout << "The sum of all boxes' GPS coordinates is " << sum_GPS_coordinates() << std::endl;
}
