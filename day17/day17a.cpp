#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>

using namespace std;

// Global variables for registers and program
int A, B, C;
vector<int> program;

// Function to extract commands from the program string
void extract_commands(const string &program_line)
{
    stringstream ss(program_line.substr(program_line.find(":") + 1));
    string num;
    while (getline(ss, num, ','))
    {
        program.push_back(stoi(num));
    }
}

// Function to read input from a file
bool read_input(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return false;
    }

    std::string lineA, lineB, lineC, empty_line, program_line;

    // Read register values
    std::getline(file, lineA);
    std::getline(file, lineB);
    std::getline(file, lineC);

    // Extract register values
    std::sscanf(lineA.c_str(), "Register A: %d", &A);
    std::sscanf(lineB.c_str(), "Register B: %d", &B);
    std::sscanf(lineC.c_str(), "Register C: %d", &C);

    // Skip empty line
    std::getline(file, empty_line);

    // Read the program
    std::getline(file, program_line);
    extract_commands(program_line);

    return true;
}

// Helper function to get the value of a combo operand
int get_combo_value(int operand)
{
    if (0 <= operand && operand <= 3)
    {
        return operand;
    }
    else if (operand == 4)
    {
        return A;
    }
    else if (operand == 5)
    {
        return B;
    }
    else if (operand == 6)
    {
        return C;
    }
    else
    {
        throw invalid_argument("Invalid combo operand encountered");
    }
}

// Function to run the program
void run_program()
{
    int pointer = 0;
    vector<int> outputs;

    while (pointer < program.size())
    {
        int opcode = program[pointer];
        int operand = program[pointer + 1];
        pointer += 2; // Move pointer past opcode and operand

        if (opcode == 0)
        { // adv
            int denominator = pow(2, get_combo_value(operand));
            if (denominator != 0)
            {
                A /= denominator;
            }
        }
        else if (opcode == 1)
        { // bxl
            B ^= operand;
        }
        else if (opcode == 2)
        { // bst
            B = get_combo_value(operand) % 8;
        }
        else if (opcode == 3)
        { // jnz
            if (A != 0)
            {
                pointer = operand; // Jump to the new instruction pointer
            }
        }
        else if (opcode == 4)
        {           // bxc
            B ^= C; // Operand ignored
        }
        else if (opcode == 5)
        { // out
            outputs.push_back(get_combo_value(operand) % 8);
        }
        else if (opcode == 6)
        { // bdv
            int denominator = pow(2, get_combo_value(operand));
            if (denominator != 0)
            {
                B = A / denominator;
            }
        }
        else if (opcode == 7)
        { // cdv
            int denominator = pow(2, get_combo_value(operand));
            if (denominator != 0)
            {
                C = A / denominator;
            }
        }
        else
        {
            throw invalid_argument("Unknown opcode encountered");
        }
    }

    // Print outputs as a comma-separated string
    for (int i = 0; i < outputs.size(); ++i)
    {
        cout << outputs[i];
        if (i < outputs.size() - 1)
        {
            cout << ",";
        }
    }
    cout << endl;
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

    run_program();
    return 0;
}
