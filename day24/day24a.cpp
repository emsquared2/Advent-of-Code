#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <tuple>

using namespace std;

map<string, int> wires;
map<string, tuple<string, string, string>> gate_rules;

void parse_line(const string &line)
{
    stringstream ss(line);
    string wireA, wireB, wireC, gate;

    ss >> wireA >> gate >> wireB;
    ss.ignore(3); // Skip "->"
    ss >> wireC;

    gate_rules[wireC] = {wireA, gate, wireB};
}

bool read_input(const string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        cerr << "Error: Could not open the file!" << endl;
        return false;
    }

    string line;
    bool reading_wires = true;
    while (getline(file, line))
    {
        if (line.empty())
        {
            reading_wires = false;
            continue;
        }

        if (reading_wires)
        {
            size_t delimiter_pos = line.find(':');
            string wire = line.substr(0, delimiter_pos);
            int value = stoi(line.substr(delimiter_pos + 2));
            wires[wire] = value;
        }
        else
        {
            parse_line(line);
        }
    }

    return true;
}

void add_wire(const string &wireA, const string &gate, const string &wireB, const string &wireC)
{
    if (wires.find(wireA) == wires.end())
    {
        tuple<string, string, string> rule = gate_rules[wireA];
        string A = get<0>(rule);
        string G = get<1>(rule);
        string B = get<2>(rule);
        add_wire(A, G, B, wireA);
    }

    if (wires.find(wireB) == wires.end())
    {
        tuple<string, string, string> rule = gate_rules[wireB];
        string A = get<0>(rule);
        string G = get<1>(rule);
        string B = get<2>(rule);
        add_wire(A, G, B, wireB);
    }

    if (gate == "AND")
    {
        wires[wireC] = wires[wireA] && wires[wireB];
    }
    else if (gate == "OR")
    {
        wires[wireC] = wires[wireA] || wires[wireB];
    }
    else if (gate == "XOR")
    {
        wires[wireC] = wires[wireA] ^ wires[wireB];
    }
    else
    {
        cerr << "Invalid gate " << gate << endl;
    }
}

// Convert binary string to decimal
uint64_t binary_to_decimal(const string &bin)
{
    uint64_t result = 0;
    for (char bit : bin)
    {
        result = result * 2 + (bit - '0');
    }
    return result;
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

    for (const auto &[key, rule] : gate_rules)
    {
        string wireA = get<0>(rule);
        string gate = get<1>(rule);
        string wireB = get<2>(rule);
        add_wire(wireA, gate, wireB, key);
    }

    set<string> z_wires;
    for (const auto &[key, _] : wires)
    {
        if (key[0] == 'z')
        {
            z_wires.insert(key);
        }
    }

    // Store the result as a binary string
    string result_bin;
    for (const auto &key : z_wires)
    {
        result_bin = to_string(wires[key]) + result_bin;
    }

    cout << binary_to_decimal(result_bin) << endl;
    return 0;
}
