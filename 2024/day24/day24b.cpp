#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>

// Function to read the input file and store the configurations
std::vector<std::string> read_input_file(const std::string &file_path)
{
    std::ifstream file(file_path);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    return lines;
}

// Function to find the gate's output wire based on its type and the x and y wires
std::string find_gate(const std::string &x_wire, const std::string &y_wire, const std::string &gate_type, const std::vector<std::string> &configurations)
{
    std::string sub_str_a = x_wire + " " + gate_type + " " + y_wire + " -> ";
    std::string sub_str_b = y_wire + " " + gate_type + " " + x_wire + " -> ";

    for (const auto &config : configurations)
    {
        if (config.find(sub_str_a) != std::string::npos || config.find(sub_str_b) != std::string::npos)
        {
            size_t pos = config.find(" -> ");
            return config.substr(pos + 4); // Extract the wire after " -> "
        }
    }
    return "";
}

// Function to swap the output wires of two gates in the configurations
std::vector<std::string> swap_output_wires(const std::string &wire_a, const std::string &wire_b, const std::vector<std::string> &configurations)
{
    std::vector<std::string> new_configurations;
    for (const auto &config : configurations)
    {
        size_t pos = config.find(" -> ");
        std::string input_wires = config.substr(0, pos);
        std::string output_wire = config.substr(pos + 4);

        if (output_wire == wire_a)
        {
            new_configurations.push_back(input_wires + " -> " + wire_b);
        }
        else if (output_wire == wire_b)
        {
            new_configurations.push_back(input_wires + " -> " + wire_a);
        }
        else
        {
            new_configurations.push_back(config);
        }
    }
    return new_configurations;
}

// Function to check the parallel adders for swapped gates
std::set<std::string> check_parallel_adders(std::vector<std::string> configurations)
{
    std::set<std::string> swaps;
    std::string current_carry_wire;
    int bit = 0;

    while (true)
    {
        std::string x_wire = std::string("x") + (bit < 10 ? "0" : "") + std::to_string(bit);
        std::string y_wire = std::string("y") + (bit < 10 ? "0" : "") + std::to_string(bit);
        std::string z_wire = std::string("z") + (bit < 10 ? "0" : "") + std::to_string(bit);

        if (bit == 0)
        {
            current_carry_wire = find_gate(x_wire, y_wire, "AND", configurations);
        }
        else
        {
            std::string ab_xor_gate = find_gate(x_wire, y_wire, "XOR", configurations);
            std::string ab_and_gate = find_gate(x_wire, y_wire, "AND", configurations);

            std::string cin_ab_xor_gate = find_gate(ab_xor_gate, current_carry_wire, "XOR", configurations);
            if (cin_ab_xor_gate.empty())
            {
                swaps.insert(ab_xor_gate);
                swaps.insert(ab_and_gate);
                configurations = swap_output_wires(ab_xor_gate, ab_and_gate, configurations);
                bit = 0;
                continue;
            }

            if (cin_ab_xor_gate != z_wire)
            {
                swaps.insert(cin_ab_xor_gate);
                swaps.insert(z_wire);
                configurations = swap_output_wires(cin_ab_xor_gate, z_wire, configurations);
                bit = 0;
                continue;
            }

            std::string cin_ab_and_gate = find_gate(ab_xor_gate, current_carry_wire, "AND", configurations);

            std::string carry_wire = find_gate(ab_and_gate, cin_ab_and_gate, "OR", configurations);
            current_carry_wire = carry_wire;
        }

        bit++;
        if (bit >= 45)
            break;
    }

    return swaps;
}

// Function to print the solution (sorted wire names involved in swaps)
void solution(const std::vector<std::string> &lines)
{
    size_t divider = 0;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        if (lines[i].empty())
        {
            divider = i;
            break;
        }
    }

    std::vector<std::string> configurations(lines.begin() + divider + 1, lines.end());

    std::set<std::string> swaps = check_parallel_adders(configurations);

    // Sort and print the swapped wires
    std::vector<std::string> sorted_swaps(swaps.begin(), swaps.end());
    std::sort(sorted_swaps.begin(), sorted_swaps.end());
    for (size_t i = 0; i < sorted_swaps.size(); ++i)
    {
        std::cout << sorted_swaps[i];
        if (i != sorted_swaps.size() - 1)
        {
            std::cout << ",";
        }
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::vector<std::string> lines = read_input_file(argv[1]);
    solution(lines);
    return 0;
}
