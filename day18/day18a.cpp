#include <fstream>
#include <vector>
#include <iostream>
#include <queue>
#include <climits>
#include <sstream>

using namespace std;

const int GRID_SIZE = 71;
const int INF = INT_MAX;

// Directions in terms of (dx, dy)
vector<pair<int, int>> directions = {
    {-1, 0}, // up
    {0, 1},  // right
    {1, 0},  // down
    {0, -1}  // left
};

vector<vector<char>> grid(GRID_SIZE, vector<char>(GRID_SIZE, '.'));

void print_grid()
{
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

bool inbounds(int row, int col)
{
    return (row >= 0 && col >= 0 && row < GRID_SIZE && col < GRID_SIZE);
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
    int bytes = 0;
    while (getline(file, line) && bytes < 1024)
    {
        stringstream ss(line);
        int x, y;
        char comma;
        if (ss >> x >> comma >> y)
        {
            if (inbounds(y, x))
            {
                grid[y][x] = '#';
            }
            ++bytes;
        }
    }
    return true;
}

// Dijkstra's algorithm to find the shortest path
int dijkstra(pair<int, int> start_pos, pair<int, int> end_pos)
{
    vector<vector<int>> dist(GRID_SIZE, vector<int>(GRID_SIZE, INF));
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;

    pq.push({0, start_pos.first, start_pos.second});
    dist[start_pos.first][start_pos.second] = 0;

    while (!pq.empty())
    {
        auto [score, x, y] = pq.top();
        pq.pop();

        if (make_pair(x, y) == end_pos)
        {
            return score;
        }

        for (auto [dx, dy] : directions)
        {
            int nx = x + dx, ny = y + dy;

            if (inbounds(nx, ny) && grid[nx][ny] != '#')
            {
                int new_cost = score + 1;
                if (new_cost < dist[nx][ny])
                {
                    dist[nx][ny] = new_cost;
                    pq.push({new_cost, nx, ny});
                }
            }
        }
    }

    return INF; // Return INF if no path is found
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

    pair<int, int> start_pos = {0, 0};
    pair<int, int> end_pos = {GRID_SIZE - 1, GRID_SIZE - 1};

    int result = dijkstra(start_pos, end_pos);
    if (result != INF)
    {
        cout << "Minimum steps: " << result << endl;
    }
    else
    {
        cout << "No path found." << endl;
    }

    return 0;
}
