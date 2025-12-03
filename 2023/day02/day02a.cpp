#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

bool isPossible(const std::string &game)
{
    // Bag limits
    static const int MAX_RED = 12;
    static const int MAX_GREEN = 13;
    static const int MAX_BLUE = 14;

    // Skip "Game X: "
    size_t start = game.find(':');
    if (start == std::string::npos)
        return false;

    std::string rest = game.substr(start + 1);

    // Split by ';' for each draw
    std::stringstream ss(rest);
    std::string draw;
    while (std::getline(ss, draw, ';'))
    {
        int red = 0, green = 0, blue = 0;

        std::stringstream ds(draw);
        std::string token;
        while (std::getline(ds, token, ','))
        {
            // Remove leading/trailing spaces
            size_t first = token.find_first_not_of(' ');
            size_t last = token.find_last_not_of(' ');
            token = token.substr(first, last - first + 1);

            // Parse number and color
            int count = 0;
            size_t pos = 0;
            while (pos < token.size() && std::isdigit(token[pos]))
                ++pos;
            count = std::stoi(token.substr(0, pos));
            std::string color = token.substr(pos);

            // Match color
            if (color.find("red") != std::string::npos)
                red = count;
            else if (color.find("green") != std::string::npos)
                green = count;
            else if (color.find("blue") != std::string::npos)
                blue = count;
        }

        // Check limits
        if (red > MAX_RED || green > MAX_GREEN || blue > MAX_BLUE)
            return false;
    }

    return true;
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

    std::string game;
    int total = 0;

    while (std::getline(file, game))
    {
        if (isPossible(game))
        {
            // Extract ID
            size_t start = game.find_first_of(' ') + 1;
            size_t end = game.find_first_of(':');
            int id = std::stoi(game.substr(start, end - start));
            total += id;
        }
    }

    std::cout << "Sum of possible game IDs: " << total << "\n";

    return 0;
}