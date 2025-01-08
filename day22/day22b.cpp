#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <limits>

using namespace std;

// Custom hash function for tuple<int, int, int, int>
namespace std
{
    template <>
    struct hash<tuple<int, int, int, int>>
    {
        size_t operator()(const tuple<int, int, int, int> &t) const
        {
            auto [a, b, c, d] = t;
            size_t h1 = hash<int>()(a);
            size_t h2 = hash<int>()(b);
            size_t h3 = hash<int>()(c);
            size_t h4 = hash<int>()(d);
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
        }
    };
}

vector<int64_t> initial_secret_numbers;

bool read_input(const string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        cerr << "Error: Could not open the file" << endl;
        return false;
    }

    string line;
    while (getline(file, line))
    {
        initial_secret_numbers.push_back(stoll(line));
    }

    file.close();
    return true;
}

void mix(int64_t &secret_number, const int64_t &value)
{
    secret_number ^= value;
}

void prune(int64_t &secret_number)
{
    secret_number %= 16777216;
}

void evolve_secret(int64_t &secret_number)
{
    int64_t temp;

    temp = secret_number * 64;
    mix(secret_number, temp);
    prune(secret_number);

    temp = secret_number / 32;
    mix(secret_number, temp);
    prune(secret_number);

    temp = secret_number * 2048;
    mix(secret_number, temp);
    prune(secret_number);
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

    unordered_map<tuple<int, int, int, int>, int64_t> sequence_map;

    for (auto initial_secret : initial_secret_numbers)
    {
        int64_t secret = initial_secret;
        vector<int> prices, changes;

        // Generate prices
        prices.push_back(secret % 10); // First price
        for (int i = 1; i < 2000; ++i)
        {
            evolve_secret(secret);
            prices.push_back(secret % 10);
        }

        // Compute price changes
        for (int i = 1; i < prices.size(); ++i)
        {
            changes.push_back(prices[i] - prices[i - 1]);
        }

        // Track sequences for the current buyer
        unordered_map<tuple<int, int, int, int>, bool> seen_for_this_buyer;

        // Map sequences of 4 changes to their first occurrence price
        for (int i = 0; i + 3 < changes.size(); ++i)
        {
            tuple<int, int, int, int> seq = {changes[i], changes[i + 1], changes[i + 2], changes[i + 3]};

            // Only consider the first occurrence of the sequence for this buyer
            if (!seen_for_this_buyer[seq])
            {
                sequence_map[seq] += prices[i + 4];
                seen_for_this_buyer[seq] = true;
            }
        }
    }

    // Find the best sequence and print it
    int64_t max_bananas = 0;
    tuple<int, int, int, int> best_sequence;

    for (const auto &[sequence, total] : sequence_map)
    {
        if (total > max_bananas)
        {
            max_bananas = total;
            best_sequence = sequence;
        }
    }

    cout << "The maximum bananas you can get is " << max_bananas << endl;
    cout << "The best sequence is: ("
         << get<0>(best_sequence) << ", "
         << get<1>(best_sequence) << ", "
         << get<2>(best_sequence) << ", "
         << get<3>(best_sequence) << ")" << endl;

    return 0;
}
