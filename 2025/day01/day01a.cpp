#include <iostream>
#include <fstream>
#include <string>

void rotate(int &res, int &dial, char dir, int distance)
{
    if (dir == 'L')
        dial = (dial - distance) % 100;
    else
        dial = (dial + distance) % 100;

    if (dial < 0)
        dial += 100; // ensure positive result

    if (dial == 0)
        ++res;
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