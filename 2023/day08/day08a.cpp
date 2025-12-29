#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

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

    int steps = 0;
    int curr_index = 0;
    int n = moves.size();
    std::string curr = "AAA";
    std::string target = "ZZZ";

    while (curr != target)
    {
        char move = moves[curr_index];
        if (move == 'L')
        {
            curr = network_map[curr].first;
        }
        else if (move == 'R')
        {
            curr = network_map[curr].second;
        }
        curr_index = (curr_index + 1) % n;
        steps++;
    }

    std::cout << steps << std::endl;

    return 0;
}