#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <queue>
#include <tuple>
#include <cmath>
#include <functional>
#include <climits>

using namespace std;

const vector<string> NUMERIC = {"789", "456", "123", " 0A"};
const vector<string> DIRECTIONAL = {" ^A", "<v>"};
vector<string> codes;

// Function to read the input from a file
bool read_input(const string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        cerr << "Error: Could not open the file " << endl;
        return false;
    }

    string line;
    while (getline(file, line))
    {
        codes.push_back(line);
    }

    return true;
}

// Forward declaration of the cost function
long long cost(const vector<string> &keypad, const string &keys, int links, unordered_map<string, long long> &memo);

// Helper function to find the position of a key on the keypad
pair<int, int> find_position(const vector<string> &keypad, char key)
{
    for (int y = 0; y < keypad.size(); y++)
    {
        for (int x = 0; x < keypad[y].size(); x++)
        {
            if (keypad[y][x] == key)
            {
                return {x, y};
            }
        }
    }
    return {-1, -1}; // Key not found
}

// Simulate walking on the keypad
bool walk(const vector<string> &keypad, int x, int y, const string &path)
{
    for (char dir : path)
    {
        // Neighbors for directions: <, >, ^, v
        pair<int, int> neighbors[] = {
            {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
        int idx = string("<>^v").find(dir);
        x = neighbors[idx].first;
        y = neighbors[idx].second;

        // Check bounds and gaps
        if (y < 0 || y >= keypad.size() || x < 0 || x >= keypad[y].size() || keypad[y][x] == ' ')
        {
            return false;
        }
    }
    return true;
}

// Find all valid shortest paths between two keys
vector<string> paths_between(const vector<string> &keypad, char start, char end)
{
    auto [x1, y1] = find_position(keypad, start);
    auto [x2, y2] = find_position(keypad, end);

    string horizontal = string(abs(x2 - x1), x2 > x1 ? '>' : '<');
    string vertical = string(abs(y2 - y1), y2 > y1 ? 'v' : '^');

    vector<string> paths;
    if (walk(keypad, x1, y1, horizontal + vertical))
    {
        paths.push_back(horizontal + vertical + "A");
    }
    if (walk(keypad, x1, y1, vertical + horizontal))
    {
        paths.push_back(vertical + horizontal + "A");
    }

    return paths;
}

// Compute the cost of moving between two keys
long long cost_between(const vector<string> &keypad, char start, char end, int links, unordered_map<string, long long> &memo)
{
    string memo_key = string(1, start) + ":" + string(1, end) + ":" + to_string(links);
    if (memo.count(memo_key))
        return memo[memo_key];

    if (links == 0)
        return 1; // Base case

    vector<string> paths = paths_between(keypad, start, end);
    long long min_cost = LLONG_MAX;

    for (const string &path : paths)
    {
        min_cost = min(min_cost, cost(DIRECTIONAL, path, links - 1, memo));
    }

    return memo[memo_key] = min_cost;
}

// Function to compute the total cost
long long cost(const vector<string> &keypad, const string &keys, int links, unordered_map<string, long long> &memo)
{
    string memo_key = keys + ":" + to_string(links);
    if (memo.count(memo_key))
        return memo[memo_key];

    long long total_cost = 0;
    char current = 'A';
    for (char next : keys)
    {
        total_cost += cost_between(keypad, current, next, links, memo);
        current = next;
    }

    return memo[memo_key] = total_cost;
}

// Calculate the complexity of a code
long long complexity(const string &code, int robots, unordered_map<string, long long> &memo)
{
    long long numeric_value = stoll(code.substr(0, code.size() - 1)); // Using stoll to handle larger numbers
    return cost(NUMERIC, code, robots + 1, memo) * numeric_value;
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

    unordered_map<string, long long> memo;
    long long total_complexity_2_robots = 0;
    long long total_complexity_25_robots = 0;

    for (const string &code : codes)
    {
        total_complexity_2_robots += complexity(code, 2, memo);
        total_complexity_25_robots += complexity(code, 25, memo);
    }

    cout << "Total complexity with 2 robots: " << total_complexity_2_robots << endl;
    cout << "Total complexity with 25 robots: " << total_complexity_25_robots << endl;

    return 0;
}
