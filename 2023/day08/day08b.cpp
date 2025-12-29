#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

long long gcd(long long a, long long b)
{
    while (b)
    {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long long lcm(long long a, long long b)
{
    return a / gcd(a, b) * b;
}

long long steps_to_z(
    const std::string &start,
    const std::string &moves,
    const std::unordered_map<std::string, std::pair<std::string, std::string>> &network)
{
    std::string node = start;
    long long steps = 0;
    int idx = 0;

    while (node[2] != 'Z')
    {
        char m = moves[idx];
        node = (m == 'L') ? network.at(node).first
                          : network.at(node).second;

        idx = (idx + 1) % moves.size();
        steps++;
    }
    return steps;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    std::string moves, empty, line;
    std::unordered_map<std::string, std::pair<std::string, std::string>> network_map;

    std::getline(file, moves);
    std::getline(file, empty); // Skip empty line
    while (std::getline(file, line))
    {
        // Extract key
        auto r = line.find(" ");
        std::string key = line.substr(0, r);

        // Extract first element
        auto l = line.find('(');
        r = line.find(',');
        std::string first_elem = line.substr(l + 1, r - l - 1);

        // Extract second element
        l = r + 2;
        r = line.find(')');
        std::string second_elem = line.substr(l, r - l);

        network_map[key] = {first_elem, second_elem};
    }

    std::vector<long long> cycles;

    for (const auto &[key, _] : network_map)
    {
        if (key[2] == 'A')
            cycles.push_back(steps_to_z(key, moves, network_map));
    }

    long long result = cycles[0];
    for (int i = 1; i < cycles.size(); ++i)
        result = lcm(result, cycles[i]);

    std::cout << result << "\n";

    return 0;
}