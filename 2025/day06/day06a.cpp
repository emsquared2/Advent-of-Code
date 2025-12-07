#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    std::vector<std::vector<long long>> numbers;
    std::vector<char> ops;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        // Operation line starts with + or *
        if (line.find_first_not_of(" *+") == std::string::npos &&
            (line.find('*') != std::string::npos || line.find('+') != std::string::npos))
        {
            std::istringstream iss(line);
            char op;
            while (iss >> op)
                ops.push_back(op);
            break; // Last line, operations only
        }

        // Numeric line
        std::istringstream iss(line);
        long long x;
        std::vector<long long> row;
        while (iss >> x)
            row.push_back(x);
        numbers.push_back(std::move(row));
    }

    int cols = ops.size();
    long long total = 0;

    for (int c = 0; c < cols; c++)
    {
        long long result = (ops[c] == '+') ? 0 : 1;

        for (auto &row : numbers)
        {
            if (ops[c] == '+')
                result += row[c];
            else
                result *= row[c];
        }

        total += result;
    }

    std::cout << "Grand total = " << total << "\n";
    return 0;
}
