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

    // Read the entire file content into a single string
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();

    // Regex patterns for combined matching
    std::regex pattern(R"(do\(\)|don't\(\)|mul\((\d+),\s*(\d+)\))");

    std::smatch match;
    int sum = 0;
    bool isEnabled = true; // Initially, do() is enabled

    // Sequentially search for matches
    auto searchStart = content.cbegin();
    auto searchEnd = content.cend();

    while (std::regex_search(searchStart, searchEnd, match, pattern))
    {
        std::string matchedString = match.str();

        // Check if it's "do()" or "don't()"
        if (matchedString == "do()")
        {
            isEnabled = true;
        }
        else if (matchedString == "don't()")
        {
            isEnabled = false;
        }
        // Check if it's "mul(A, B)" (regex captures groups)
        else if (match.size() == 3)
        {
            if (isEnabled)
            {
                int A = std::stoi(match[1].str());
                int B = std::stoi(match[2].str());
                sum += A * B;
            }
        }

        // Move past the current match
        searchStart = match.suffix().first;
    }

    std::cout << "Total sum is: " << sum << std::endl;
    return 0;
}
