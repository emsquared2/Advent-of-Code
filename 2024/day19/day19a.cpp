#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

unordered_set<string> towel_patterns;
vector<string> designs;

// Helper function to trim whitespace
string trim(const string &str)
{
    int start = str.find_first_not_of(" \t");
    int end = str.find_last_not_of(" \t");
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
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
    bool reading_patterns = true;

    // Read towel patterns and designs
    while (getline(file, line))
    {
        line = trim(line);
        if (line.empty())
        {
            reading_patterns = false;
            continue;
        }

        if (reading_patterns)
        {
            stringstream ss(line);
            string pattern;
            while (getline(ss, pattern, ','))
            {
                towel_patterns.insert(trim(pattern));
            }
        }
        else
        {
            designs.push_back(line);
        }
    }

    return true;
}

bool can_construct(const string &design, const unordered_set<string> &towel_patterns)
{
    int n = design.size();

    // dp[i] indicates whether the first i characters of the design can be constructed
    vector<bool> dp(n + 1, false);

    // Base case: An empty design can always be constructed
    dp[0] = true;

    for (int i = 1; i <= n; ++i)
    {
        // Check all possible substrings ending at position i
        for (int j = 0; j < i; ++j)
        {
            // Extract the substring from position j to i-1
            string sub = design.substr(j, i - j);

            // If the first j characters can be constructed (dp[j] is true)
            // and the current substring exists in towel_patterns,
            // mark dp[i] as true (the first i characters can be constructed)
            if (dp[j] && towel_patterns.count(sub))
            {
                dp[i] = true;
                break; // No need to check further substrings for this position
            }
        }
    }

    return dp[n];
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

    int possible_count = 0;
    for (const string &design : designs)
    {
        if (can_construct(design, towel_patterns))
        {
            ++possible_count;
        }
    }

    cout << "Possible designs: " << possible_count << endl;

    return 0;
}
