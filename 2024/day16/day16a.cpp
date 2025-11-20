#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <tuple>
#include <climits>

std::vector<std::vector<char>> map;

bool inbounds(int row, int col)
{
    return (row >= 0 && col >= 0 && row < map.size() && col < map[0].size());
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
    while (std::getline(file, line))
    {
        std::vector<char> row(line.begin(), line.end());
        map.push_back(row);
    }

    file.close();
    return true;
}

std::pair<int, int> find_start()
{
    for (int i = 0; i < map.size(); ++i)
    {
        for (int j = 0; j < map[i].size(); ++j)
        {
            if (map[i][j] == 'S')
            {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

// Direction offsets: {dx, dy, direction_char}
const std::vector<std::tuple<int, int, char>> directions = {
    {0, 1, '>'},  // East
    {1, 0, 'v'},  // South
    {0, -1, '<'}, // West
    {-1, 0, '^'}  // North
};

// BFS with priority queue to find the lowest score
int find_lowest_score(int start_x, int start_y)
{
    // Priority queue: {cost, x, y, direction_index}
    std::priority_queue<std::tuple<int, int, int, int>,
                        std::vector<std::tuple<int, int, int, int>>,
                        std::greater<std::tuple<int, int, int, int>>>
        pq;

    // DP array to track the minimum cost for (x, y, direction)
    int rows = map.size(), cols = map[0].size();
    std::vector<std::vector<std::vector<int>>> dp(rows, std::vector<std::vector<int>>(cols, std::vector<int>(4, INT_MAX)));

    // Initialize starting point
    pq.emplace(0, start_x, start_y, 0);
    dp[start_x][start_y][0] = 0;

    while (!pq.empty())
    {
        auto [cost, x, y, dir] = pq.top();
        pq.pop();

        // If we reach the endpoint, return the cost
        if (map[x][y] == 'E')
            return cost;

        // If this state has been processed with a lower cost, skip it
        if (cost > dp[x][y][dir])
            continue;

        // Try moving forward
        auto [dx, dy, _] = directions[dir];
        int nx = x + dx, ny = y + dy;
        if (inbounds(nx, ny) && map[nx][ny] != '#' && cost + 1 < dp[nx][ny][dir])
        {
            dp[nx][ny][dir] = cost + 1;
            pq.emplace(cost + 1, nx, ny, dir);
        }

        // Try turning clockwise or counterclockwise
        for (int turn : {-1, 1})
        {
            int new_dir = (dir + turn + 4) % 4; // Ensure new_dir is in [0, 3]
            if (cost + 1000 < dp[x][y][new_dir])
            {
                dp[x][y][new_dir] = cost + 1000;
                pq.emplace(cost + 1000, x, y, new_dir);
            }
        }
    }

    return INT_MAX; // No valid path found
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

    auto [start_x, start_y] = find_start();
    if (start_x == -1 || start_y == -1)
    {
        std::cerr << "Error: Start position not found!" << std::endl;
        return 1;
    }

    int result = find_lowest_score(start_x, start_y);
    if (result == INT_MAX)
        std::cout << "No valid path to the endpoint found!" << std::endl;
    else
        std::cout << "The lowest score a Reindeer could possibly get is " << result << std::endl;

    return 0;
}
