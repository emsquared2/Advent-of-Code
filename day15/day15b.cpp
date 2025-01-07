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

void adjust_map()
{
    for (int i = 0; i < map.size(); ++i)
    {
        std::vector<char> new_row;
        for (int j = 0; j < map[i].size(); ++j)
        {
            switch (map[i][j])
            {
            case '#':
                new_row.push_back('#');
                new_row.push_back('#');
                break;
            case 'O':
                new_row.push_back('[');
                new_row.push_back(']');
                break;
            case '.':
                new_row.push_back('.');
                new_row.push_back('.');
                break;
            case '@':
                new_row.push_back('@');
                new_row.push_back('.');
                break;
            default:
                break;
            }
        }
        map[i] = new_row;
    }
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

// Helper function to handle the update of the box positions based on movement direction.
void update_box_positions(int box1_x, int box1_y, int box2_x, int box2_y, int dx, int dy)
{
    if (dx != 0) // Moving up or down
    {
        // Both parts should become empty
        map[box1_x][box1_y] = '.';
        map[box2_x][box2_y] = '.';
    }
    else if (dy == -1) // Moving left
    {
        if (box1_y < box2_y)
        {
            map[box2_x][box2_y] = '.';
        }
        else
            map[box1_x][box1_y] = '.';
    }
    else if (dy == 1) // Moving right
    {
        if (box1_y > box2_y)
        {
            map[box2_x][box2_y] = '.';
        }
        else
            map[box1_x][box1_y] = '.';
    }
}

bool horizontal_push(int x, int y, int dy)
{
    int box_x1 = x, box_y1 = y;
    int box_x2 = x, box_y2 = y + dy;

    // Calculate new positions for both parts of the box
    int new_box_x1 = box_x1, new_box_y1 = box_y1 + dy;
    int new_box_x2 = box_x2, new_box_y2 = box_y2 + dy;

    // Base case: if out of bounds or wall, abort the push
    if (!inbounds(new_box_x2, new_box_y2) || map[new_box_x2][new_box_y2] == '#')
        return false;

    // If it's an empty space, we can push both parts of the box
    if (map[new_box_x2][new_box_y2] == '.')
    {
        map[new_box_x2][new_box_y2] = map[box_x2][box_y2];
        map[new_box_x1][new_box_y1] = map[box_x1][box_y1];

        // Update the positions using the helper method
        update_box_positions(box_x1, box_y1, box_x2, box_y2, 0, dy);
        return true;
    }
    // If it's another box, attempt to push it further
    if (map[new_box_x2][new_box_y2] == '[' || map[new_box_x2][new_box_y2] == ']')
    {
        // Try to push that box first
        if (horizontal_push(new_box_x2, new_box_y2, dy))
        {
            // After the push, update the positions
            map[new_box_x2][new_box_y2] = map[box_x2][box_y2];
            map[new_box_x1][new_box_y1] = map[box_x1][box_y1];

            // Update positions using the helper method
            update_box_positions(box_x1, box_y1, box_x2, box_y2, 0, dy);
            return true;
        }
    }
    return false;
}

bool vertical_push(int x, int y, int dx)
{
    int box_x1 = x, box_y1 = y;
    int box_x2 = x, box_y2 = (map[x][y] == '[') ? (y + 1) : (y - 1);

    // Calculate new positions for both parts of the box
    int new_box_x1 = box_x1 + dx, new_box_y1 = box_y1;
    int new_box_x2 = box_x2 + dx, new_box_y2 = box_y2;

    // Base case: if out of bounds or wall, abort the push
    if (!inbounds(new_box_x1, new_box_y1) || map[new_box_x1][new_box_y1] == '#' ||
        !inbounds(new_box_x2, new_box_y2) || map[new_box_x2][new_box_y2] == '#')
        return false;

    // If it's an empty space, we can push both parts of the box
    if (map[new_box_x1][new_box_y1] == '.' && map[new_box_x2][new_box_y2] == '.')
    {
        map[new_box_x2][new_box_y2] = map[box_x2][box_y2];
        map[new_box_x1][new_box_y1] = map[box_x1][box_y1];

        // Update the positions using the helper method
        update_box_positions(box_x1, box_y1, box_x2, box_y2, dx, 0);
        return true;
    }

    /*  ..[][]..   ..[].. ..[].. ..[]..
     *  ...[]...   ..[].. .[]... ...[]..
     */
    // If it's another box, attempt to push it further
    if (map[new_box_x1][new_box_y1] == '[' || map[new_box_x1][new_box_y1] == ']' || map[new_box_x2][new_box_y2] == '[' || map[new_box_x2][new_box_y2] == ']')
    {
        // If they are perfectly aligned
        if (map[box_x1][box_y1] == map[new_box_x1][new_box_y1] && map[box_x2][box_y2] == map[new_box_x2][new_box_y2])
        {
            // Try to push that box first
            if (vertical_push(new_box_x2, new_box_y2, dx))
            {
                // After the push, update the positions
                map[new_box_x2][new_box_y2] = map[box_x2][box_y2];
                map[new_box_x1][new_box_y1] = map[box_x1][box_y1];

                // Update positions using the helper method
                update_box_positions(box_x1, box_y1, box_x2, box_y2, dx, 0);
                return true;
            }
        }
        // If there is one box not aligned
        else if (map[new_box_x1][new_box_y1] == '.' || map[new_box_x2][new_box_y2] == '.')
        {
            if (map[new_box_x1][new_box_y1] == '.')
            {
                // Try to push that box first
                if (vertical_push(new_box_x2, new_box_y2, dx))
                {
                    // After the push, update the positions
                    map[new_box_x2][new_box_y2] = map[box_x2][box_y2];
                    map[new_box_x1][new_box_y1] = map[box_x1][box_y1];

                    // Update positions using the helper method
                    update_box_positions(box_x1, box_y1, box_x2, box_y2, dx, 0);
                    return true;
                }
            }
            else
            {
                // Try to push that box first
                if (vertical_push(new_box_x1, new_box_y1, dx))
                {
                    // After the push, update the positions
                    map[new_box_x2][new_box_y2] = map[box_x2][box_y2];
                    map[new_box_x1][new_box_y1] = map[box_x1][box_y1];

                    // Update positions using the helper method
                    update_box_positions(box_x1, box_y1, box_x2, box_y2, dx, 0);
                    return true;
                }
            }
        }
        // If there are 2 boxes
        else if (map[box_x1][box_y1] != map[new_box_x1][new_box_y1] && map[new_box_x1][new_box_y1] != '.' &&
                 map[box_x2][box_y2] != map[new_box_x2][new_box_y2] && map[new_box_x2][new_box_y2] != '.')
        {
            // Create a copy of the map
            std::vector<std::vector<char>> mapCopy = map;

            // Try to push them both
            if (vertical_push(new_box_x1, new_box_y1, dx) && vertical_push(new_box_x2, new_box_y2, dx))
            {
                // After the push, update the positions
                map[new_box_x2][new_box_y2] = map[box_x2][box_y2];
                map[new_box_x1][new_box_y1] = map[box_x1][box_y1];

                // Update positions using the helper method
                update_box_positions(box_x1, box_y1, box_x2, box_y2, dx, 0);
                return true;
            }
            else
            {
                map = mapCopy;
                return false;
            }
        }
    }
    return false;
}

bool push_boxes(int x, int y, int dx, int dy)
{
    if (dy != 0)
        return horizontal_push(x, y, dy);
    else
        return vertical_push(x, y, dx);
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
    else if (map[new_x][new_y] == '[' || map[new_x][new_y] == ']')
    {
        // Attempt to push wide box
        if (push_boxes(new_x, new_y, dx, dy))
        {
            // If successful, move the robot
            map[robot_x][robot_y] = '.';
            map[new_x][new_y] = '@';
            robot_x = new_x;
            robot_y = new_y;
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
            if (map[i][j] == '[')
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

    adjust_map();

    int robot_x, robot_y;
    std::pair<int, int> initial_location = find_robot();
    robot_x = initial_location.first;
    robot_y = initial_location.second;

    // print_map();

    for (const char c : moves)
    {
        // std::cout << "Move " << c << std::endl;
        move(c, robot_x, robot_y);
        // print_map();
    }

    print_map();

    std::cout << "The sum of all boxes' GPS coordinates is " << sum_GPS_coordinates() << std::endl;

    return 0;
}
