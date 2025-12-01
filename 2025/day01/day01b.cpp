#include <iostream>
#include <fstream>
#include <string>

void rotate(int &res, int &dial, char dir, int distance)
{
    // Count full wraps
    res += distance / 100;
    distance %= 100;

    int start = dial;
    int end;

    if (dir == 'L')
    {
        end = dial - distance;
        if (end <= 0 && start > 0)
            ++res;
        dial = (end % 100 + 100) % 100;
    }
    else
    {
        end = dial + distance;
        if (end >= 100 && start < 100)
            ++res;
        dial = end % 100;
    }
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

    std::string rotation;
    int dial = 50;
    int res = 0;

    while (file >> rotation)
    {
        char dir = rotation[0];
        int distance = std::stoi(rotation.substr(1));

        rotate(res, dial, dir, distance);
    }

    std::cout << "The actul password to open the door is " << res << "\n";

    return 0;
}