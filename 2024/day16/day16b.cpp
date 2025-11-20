#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include <set>
#include <tuple>

using namespace std;

const int INF = INT_MAX;
const int ROTATION_COST = 1000; // Cost for changing direction (turning)
const int MOVE_COST = 1;        // Cost for moving forward

// Directions in terms of (dx, dy)
vector<pair<int, int>> directions = {
    {-1, 0}, // North
    {0, 1},  // East
    {1, 0},  // South
    {0, -1}  // West
};

// Maze representation and start/end positions
vector<string> maze;
int rows, cols;
pair<int, int> start_pos, end_pos;

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
        maze.push_back(line);
    }

    file.close();

    rows = maze.size();
    cols = maze[0].size();

    // Find the start ('S') and end ('E') positions
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (maze[i][j] == 'S')
            {
                start_pos = {i, j};
            }
            else if (maze[i][j] == 'E')
            {
                end_pos = {i, j};
            }
        }
    }

    return true;
}

// Dijkstra's algorithm to find the shortest path
int dijkstra()
{
    // Initialize the seen array with max size values to track the shortest distances
    vector<vector<vector<int>>> seen(rows, vector<vector<int>>(cols, vector<int>(4, INF)));

    // Priority queue for Dijkstra-like algorithm (min-heap)
    // The tuple contains (score, y, x, direction, path)
    priority_queue<tuple<int, int, int, int, vector<pair<int, int>>>,
                   vector<tuple<int, int, int, int, vector<pair<int, int>>>>,
                   greater<tuple<int, int, int, int, vector<pair<int, int>>>>>
        pq;

    // Start from the start position, initially facing East (direction 1)
    pq.push({0, start_pos.first, start_pos.second, 1, {start_pos}});

    vector<vector<pair<int, int>>> paths; // To store the different paths found
    int best_score = INF;                 // Store the best score (shortest path length)

    // While the priority queue is not empty and the best score hasn't been reached
    while (!pq.empty() && get<0>(pq.top()) <= best_score)
    {
        int score = get<0>(pq.top());                   // Current score (path cost)
        int x = get<1>(pq.top());                       // Current x (row) position
        int y = get<2>(pq.top());                       // Current y (column) position
        int dir = get<3>(pq.top());                     // Current direction (0: North, 1: East, 2: South, 3: West)
        vector<pair<int, int>> path = get<4>(pq.top()); // The current path leading to (x, y)
        pq.pop();                                       // Remove the current position from the priority queue

        // If we've reached the end position, store the current path and its score
        if (make_pair(x, y) == end_pos)
        {
            best_score = score;    // Update the best score
            paths.push_back(path); // Store the path
            continue;              // Skip further processing if the end is found
        }

        // Skip if a better path has already been found to (y, x) in the same direction
        if (seen[x][y][dir] < score)
        {
            continue;
        }
        seen[x][y][dir] = score; // Mark the current position as visited with the best score

        // Explore all four possible directions (N, E, S, W)
        for (int i = 0; i < 4; ++i)
        {
            int dx = directions[i].first;  // x direction (horizontal movement)
            int dy = directions[i].second; // y direction (vertical movement)
            int nx = x + dx, ny = y + dy;  // Calculate the new position

            // Check if the new position is valid (within bounds and not a wall '#')
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] != '#' &&
                find(path.begin(), path.end(), make_pair(nx, ny)) == path.end()) // Avoid revisiting the same position
            {
                // If we're changing direction, add a rotation cost; otherwise, add a move cost
                int cost = (i == dir) ? MOVE_COST : ROTATION_COST;

                vector<pair<int, int>> new_path = path;       // Create a new path
                new_path.push_back({nx, ny});                 // Add the new position to the path
                pq.push({score + cost, nx, ny, i, new_path}); // Push the new state into the priority queue
            }
        }
    }

    // Collect all unique visited positions (seats) and return their count
    set<pair<int, int>> seats;
    for (const auto &path : paths)
    {
        seats.insert(path.begin(), path.end()); // Insert all positions from the path into the set (removes duplicates)
    }

    return seats.size(); // Return the total number of unique seats visited
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

    std::cout << dijkstra() << " tiles are part of at least one of the best paths through the maze." << std::endl;

    return 0;
}
