#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

int gamePower(const std::string &game)
{
    // Skip "Game X: "
    size_t start = game.find(':');
    if (start == std::string::npos)
        return 0; // invalid line

    std::string rest = game.substr(start + 1);

    int red = 0, green = 0, blue = 0;

    // Split by ';' for each draw
    std::stringstream ss(rest);
    std::string draw;
    while (std::getline(ss, draw, ';'))
    {
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

            // Update maximum needed per color
            if (color.find("red") != std::string::npos)
                red = std::max(red, count);
            else if (color.find("green") != std::string::npos)
                green = std::max(green, count);
            else if (color.find("blue") != std::string::npos)
                blue = std::max(blue, count);
        }
    }

    return red * green * blue;
}

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
        std::cerr << "Error: Could not open file\n";
        return 1;
    }

    std::string game;
    long long total = 0;

    while (std::getline(file, game))
    {
        total += gamePower(game);
    }

    std::cout << "The sum of the power is " << total << "\n";
}
