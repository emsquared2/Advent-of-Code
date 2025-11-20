#include <iostream>
#include <fstream>
#include <regex>
#include <string>

int main(int argc, char *argv[])
{
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

    std::regex pattern(R"(mul\((\d+),\s*(\d+)\))");
    std::string line;
    std::smatch match;
    int sum = 0;

    while (std::getline(file, line))
    {
        // Search for the pattern in the line
        auto lineBegin = line.cbegin();
        auto lineEnd = line.cend();

        while (std::regex_search(lineBegin, lineEnd, match, pattern))
        {
            int A = std::stoi(match[1].str());
            int B = std::stoi(match[2].str());
            sum += A * B;

            lineBegin = match.suffix().first; // Move past the current match
        }
    }

    file.close();

    std::cout << "Total sum is: " << sum << std::endl;
    return 0;
}
