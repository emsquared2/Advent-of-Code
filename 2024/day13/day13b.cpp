#include <fstream>
#include <iostream>
#include <vector>
#include <climits>

struct claw_machine
{
    long long xA, xB;
    long long yA, yB;
    long long xPrize, yPrize;
};

const long long BUTTON_A_COST = 3;
const long long BUTTON_B_COST = 1;

std::vector<claw_machine> machines;

bool read_input(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return false;
    }

    std::string lineA, lineB, prize, empty_line;

    while (std::getline(file, lineA) && std::getline(file, lineB) && std::getline(file, prize))
    {
        claw_machine machine;

        // Extract Button A behavior (e.g., "X+75, Y+30")
        if (std::sscanf(lineA.c_str(), "Button A: X+%lld, Y+%lld", &machine.xA, &machine.yA) != 2)
        {
            std::cerr << "Error: Invalid format for Button A!" << std::endl;
            return false;
        }

        // Extract Button B behavior (e.g., "Button B: X+57, Y+75")
        if (std::sscanf(lineB.c_str(), "Button B: X+%lld, Y+%lld", &machine.xB, &machine.yB) != 2)
        {
            std::cerr << "Error: Invalid format for Button B!" << std::endl;
            return false;
        }

        // Extract Prize location (e.g., "Prize: X=9885, Y=8130")
        if (std::sscanf(prize.c_str(), "Prize: X=%lld, Y=%lld", &machine.xPrize, &machine.yPrize) != 2)
        {
            std::cerr << "Error: Invalid format for Prize!" << std::endl;
            return false;
        }

        // Add 10000000000000 to both X and Y prize locations
        machine.xPrize += 10000000000000;
        machine.yPrize += 10000000000000;

        // Skip empty line
        std::getline(file, empty_line);

        machines.push_back(machine);
    }

    file.close();
    return true;
}

/* For each claw machine, there are two equations:
 *      1) xA * a_pressed + xB * b_pressed = xPrize
 *      2) yA * a_pressed + yB * b_pressed = yPrize
 *
 * Notation:
 *      xA --> a1, xB --> b1, c1 --> xPrize
 *      yA --> a2, yB --> b2, c2 --> yPrize
 *      a_pressed --> x, b_pressed --> y
 *
 * => a1 * x + b1 * y = c1
 * => a2 * x + b2 * y = c2
 *
 * This is a 2x2 system. We use Cramer's rule:
 *
 * x = (c1 * b2 - b1 * c2) / (a1 * b2 - b1 * a2)
 * y = (a1 * c2 - c1 * a2) / (a1 * b2 - b1 * a2)
 *
 * To ensure the solution is valid:
 *      1. denominator != 0 (system has a unique solution)
 *      2. x and y are integers and non-negative
 */

long long tokens_to_win(const claw_machine &machine)
{
    long long denominator = machine.xA * machine.yB - machine.xB * machine.yA;

    // If the denominator is zero, the equations are dependent or unsolvable
    if (denominator == 0)
    {
        return 0; // No valid solution
    }

    // Calculate the numerators for Cramer's rule
    long long numerator_x = machine.xPrize * machine.yB - machine.xB * machine.yPrize;
    long long numerator_y = machine.xA * machine.yPrize - machine.xPrize * machine.yA;

    // Check if the numerators are cleanly divisible by the denominator
    if (numerator_x % denominator != 0 || numerator_y % denominator != 0)
    {
        return 0; // No integer solution exists
    }

    // Calculate a_pressed (x) and b_pressed (y)
    long long a_pressed = numerator_x / denominator;
    long long b_pressed = numerator_y / denominator;

    // Ensure the solution is non-negative
    if (a_pressed < 0 || b_pressed < 0)
    {
        return 0; // No valid solution with non-negative button presses
    }

    // Calculate the total token cost
    return a_pressed * BUTTON_A_COST + b_pressed * BUTTON_B_COST;
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

    long long total_tokens = 0;

    // For each machine, calculate the minimum tokens to win the prize
    for (const auto &machine : machines)
    {
        long long tokens = tokens_to_win(machine);
        total_tokens += tokens;
    }

    std::cout << "The fewest tokens you would have to spend to win all possible prizes is " << total_tokens << " tokens" << std::endl;
    return 0;
}
