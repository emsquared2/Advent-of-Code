#include <iostream>
#include <fstream>
#include <string>
#include <vector>

static const std::vector<std::pair<std::string, int>> DIGITS = {
    {"zero", 0}, {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};

int calibrationValue(const std::string &s)
{
    int first = -1;
    int last = -1;

    for (int i = 0; i < s.size(); ++i)
    {
        int val = -1;

        // Numeric digit
        if (std::isdigit(s[i]))
        {
            val = s[i] - '0';
        }
        else
        {
            // Spelled-out digits
            for (const auto &p : DIGITS)
            {
                const std::string &word = p.first;
                if (i + word.size() <= s.size() &&
                    s.compare(i, word.size(), word) == 0)
                {
                    val = p.second;
                    break;
                }
            }
        }

        if (val != -1)
        {
            if (first == -1)
                first = val;
            last = val;
        }
    }

    return first * 10 + last;
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
    int res = 0;

    while (file >> line)
    {
        res += calibrationValue(line);
    }

    std::cout << "The sum of all of the calibration values is " << res << "\n";

    return 0;
}