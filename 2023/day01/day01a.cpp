#include <iostream>
#include <fstream>
#include <string>

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
        int first = -1;
        int last = -1;

        for (char c : line)
        {
            if (std::isdigit(c))
            {
                int d = c - '0';
                if (first == -1)
                    first = d;
                last = d;
            }
        }

        if (first != -1) // safety check: line contains at least one digit
            res += first * 10 + last;
    }

    std::cout << "The sum of all of the calibration values is " << res << "\n";

    return 0;
}