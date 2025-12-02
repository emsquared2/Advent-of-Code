#include <iostream>
#include <fstream>
#include <string>

bool isDouble(const std::string &s)
{
    int n = s.size();
    if (n % 2 != 0)
        return false; // must be even length

    int half = n / 2;
    return s.substr(0, half) == s.substr(half, half);
}

int main(int argc, char *argv[])
{
    // Check if the correct number of arguments is provided
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return 1;
    }

    std::string line;
    std::getline(file, line);

    std::size_t start = 0;
    long long invalidIDsSum = 0;

    while (start < line.length())
    {
        // Find '-' separator
        std::size_t dash = line.find('-', start);
        if (dash == std::string::npos)
            break;

        long long first = std::stoll(line.substr(start, dash - start));
        start = dash + 1;

        // Find ',' separator (may not exist)
        std::size_t comma = line.find(',', start);

        long long second;
        if (comma == std::string::npos)
        {
            // Last range
            second = std::stoll(line.substr(start));
            start = line.length(); // end loop
        }
        else
        {
            second = std::stoll(line.substr(start, comma - start));
            start = comma + 1;
        }

        // Process the range
        for (long long i = first; i <= second; ++i)
        {
            std::string id = std::to_string(i);
            if (isDouble(id))
                invalidIDsSum += i;
        }
    }
    std::cout << "The sum of all invalid IDs is " << invalidIDsSum << "\n";

    return 0;
}