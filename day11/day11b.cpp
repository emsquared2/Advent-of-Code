#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

bool read_input(const std::string &filepath, std::map<long long, long long> &stone_counts)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return false;
    }

    long long stone;
    while (file >> stone)
    {
        stone_counts[stone] += 1;
    }

    file.close();
    return true;
}

// Fast powers of 10 for up to 18 digits
long long fast_pow10(int exp)
{
    static const long long powers_of_10[] = {
        1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
        1000000000, 10000000000, 100000000000, 1000000000000,
        10000000000000, 100000000000000, 1000000000000000,
        10000000000000000, 100000000000000000, 1000000000000000000};
    return powers_of_10[exp];
}

void blink(std::map<long long, long long> &stone_counts)
{
    std::map<long long, long long> next_counts;

    for (const auto &[stone, count] : stone_counts)
    {
        if (stone == 0)
        {
            next_counts[1] += count; // Replace 0 with 1
        }
        else
        {
            // Count digits
            int num_digits = 0;
            long long temp = stone;
            while (temp)
            {
                temp /= 10;
                ++num_digits;
            }

            if (num_digits % 2 == 0) // Even digits - split
            {
                long long divisor = fast_pow10(num_digits / 2);
                long long left = stone / divisor;
                long long right = stone % divisor;

                next_counts[left] += count;
                next_counts[right] += count;
            }
            else // Odd digits - multiply
            {
                next_counts[stone * 2024] += count;
            }
        }
    }

    stone_counts = std::move(next_counts);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::map<long long, long long> stone_counts;

    if (!read_input(argv[1], stone_counts))
        return 1;

    const int max_iterations = 75;
    for (int i = 0; i < max_iterations; ++i)
    {
        blink(stone_counts);
    }

    long long total_stones = 0;
    for (const auto &[stone, count] : stone_counts)
    {
        total_stones += count;
    }

    std::cout << "There are " << total_stones << " stones after " << max_iterations << " blinks." << std::endl;

    return 0;
}
