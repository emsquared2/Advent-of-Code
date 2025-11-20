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
    long long start = str.find_first_not_of(" \t");
    long long end = str.find_last_not_of(" \t");
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

long long count_ways_to_construct(const string &design, const unordered_set<string> &towel_patterns)
{
    long long n = design.size();

    // dp[i] represents the number of ways to construct the first i characters of the design
    vector<long long> dp(n + 1, 0);

    // Base case: There's one way to construct an empty design
    dp[0] = 1;

    for (long long i = 1; i <= n; ++i)
    {
        // Check all possible substrings ending at position i
        for (long long j = 0; j < i; ++j)
        {
            // Extract the substring from position j to i-1
            string sub = design.substr(j, i - j);

            // If the substring matches a towel pattern, add the number of ways
            // to construct the first j characters to dp[i]
            if (towel_patterns.count(sub))
            {
                dp[i] += dp[j];
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

    long long total_ways = 0;
    for (const string &design : designs)
    {
        total_ways += count_ways_to_construct(design, towel_patterns);
    }

    cout << "Total number of ways to arrange towels: " << total_ways << endl;

    return 0;
}
