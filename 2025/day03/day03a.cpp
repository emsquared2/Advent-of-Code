#include <iostream>
#include <fstream>
#include <string>

int joltage(std::string &bank)
{
    int first = 0;
    int idx = 0;
    int second = 0;
    int n = bank.size();

    for (int i = 0; i < n - 1; ++i)
    {
        if (bank[i] - '0' > first)
            idx = i;
        first = std::max(first, bank[i] - '0');
    }
    for (int i = idx + 1; i < n; ++i)
        second = std::max(second, bank[i] - '0');

    return first * 10 + second;
}

int main(int argc, char *argv[])
{
    // Check if the correct number of arguments is provided
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << "file_path" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error: Could not open the file>" << std::endl;
        return 1;
    }

    std::string bank;
    int totalJoltage = 0;
    while (std::getline(file, bank))
    {
        totalJoltage += joltage(bank);
    }

    std::cout << "The total output joltage is " << totalJoltage << "\n";
    return 0;
}