#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

bool read_input(const std::string &filepath, std::multimap<long long, std::vector<long long>> &data)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return false;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        long long key;
        char colon;
        iss >> key >> colon;

        std::vector<long long> values;
        long long value;
        while (iss >> value)
        {
            values.push_back(value);
        }

        data.insert({key, values});
    }

    file.close();

    return true;
}

long long concatenation(long left, long right)
{
    return std::stoll(std::to_string(left) + std::to_string(right));
}

// Recursive helper function
bool check_combinations(const std::vector<long long> &values, int current_index, long long current_result, const long long target)
{
    // Base case: if we've used all numbers, check if the result matches the target
    if (current_index == values.size())
    {
        return current_result == target;
    }

    // Try the next number with addition
    if (check_combinations(values, current_index + 1, current_result + values[current_index], target))
    {
        return true;
    }

    // Try the next number with multiplication
    if (check_combinations(values, current_index + 1, current_result * values[current_index], target))
    {
        return true;
    }

    // Try with concatenation between current result and next number
    if (check_combinations(values, current_index + 1, concatenation(current_result, values[current_index]), target))
    {
        return true;
    }

    // If neither operation works, return false
    return false;
}

bool valid_equation(const long long result, const std::vector<long long> &values)
{
    if (values.empty())
    {
        return false;
    }
    return check_combinations(values, 1, values[0], result);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::multimap<long long, std::vector<long long>> data;
    if (!read_input(argv[1], data))
        return 1;

    long long total_calibration = 0;

    for (const auto &[result, values] : data)
    {
        if (valid_equation(result, values))
            total_calibration += result;
    }

    std::cout << "The total calibration result is " << total_calibration << std::endl;

    return 0;
}
