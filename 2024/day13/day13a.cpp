#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>

struct claw_machine
{
    int xA, xB;
    int yA, yB;
    int xPrize, yPrize;

    void print()
    {
        std::cout << "Button A --> X: " << xA << ", Y: " << yA << std::endl;
        std::cout << "Button B --> X: " << xB << ", Y: " << yB << std::endl;
        std::cout << "Prize --> X: " << xPrize << ", Y: " << yPrize << std::endl;
    }
};

const int BUTTON_A_COST = 3;
const int BUTTON_B_COST = 1;

// Memoization map
std::unordered_map<std::string, int> memo;

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
        std::sscanf(lineA.c_str(), "Button A: X+%d, Y+%d", &machine.xA, &machine.yA);

        // Extract Button B behavior (e.g., "Button B: X+57, Y+75")
        std::sscanf(lineB.c_str(), "Button B: X+%d, Y+%d", &machine.xB, &machine.yB);

        // Extract Prize location (e.g., "Prize: X=9885, Y=8130")
        std::sscanf(prize.c_str(), "Prize: X=%d, Y=%d", &machine.xPrize, &machine.yPrize);

        // Skip empty line
        std::getline(file, empty_line);

        // machine.print();
        machines.push_back(machine);
    }

    file.close();
    return true;
}

std::string generate_key(int A_pressed, int B_pressed, int current_x, int current_y)
{
    return std::to_string(A_pressed) + "," + std::to_string(B_pressed) + "," + std::to_string(current_x) + "," + std::to_string(current_y);
}

int tokens_to_win(const claw_machine &machine, int A_pressed, int B_pressed, int current_tokens, int current_x, int current_y)
{
    // If we have already computed this state, return the stored result
    std::string key = generate_key(A_pressed, B_pressed, current_x, current_y);
    if (memo.find(key) != memo.end())
    {
        return memo[key];
    }

    // Base case: when we reach the prize
    if (current_x == machine.xPrize && current_y == machine.yPrize)
    {
        return current_tokens;
    }

    // If both buttons have been pressed 100 times, return a large value (indicating we can't proceed)
    if (A_pressed == 100 && B_pressed == 100)
    {
        return INT_MAX;
    }

    // Try all possible actions and minimize the token cost
    int min_tokens = INT_MAX;

    // Try pressing Button A
    if (A_pressed < 100)
    {
        int new_tokens = current_tokens + BUTTON_A_COST;
        int newX = current_x + machine.xA;
        int newY = current_y + machine.yA;
        min_tokens = std::min(min_tokens, tokens_to_win(machine, A_pressed + 1, B_pressed, new_tokens, newX, newY));
    }

    // Try pressing Button B
    if (B_pressed < 100)
    {
        int new_tokens = current_tokens + BUTTON_B_COST;
        int newX = current_x + machine.xB;
        int newY = current_y + machine.yB;
        min_tokens = std::min(min_tokens, tokens_to_win(machine, A_pressed, B_pressed + 1, new_tokens, newX, newY));
    }

    // Store the result in memoization map and return
    memo[key] = min_tokens;
    return min_tokens;
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

    int total_tokens = 0;

    // For each machine, calculate the minimum tokens to win the prize
    for (const auto &machine : machines)
    {
        int tokens = tokens_to_win(machine, 0, 0, 0, 0, 0);
        total_tokens += (tokens == INT_MAX) ? 0 : tokens;
        memo.clear();
    }

    std::cout << "The fewest tokens you would have to spend to win all possible prizes is " << total_tokens << " tokens" << std::endl;
    return 0;
}
