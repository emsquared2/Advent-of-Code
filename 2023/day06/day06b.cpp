#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int travel(int time, int distance)
{
    int res = 0;

    for (int wait = 0; wait <= time; ++wait)
    {
        if (wait * (time - wait) > distance)
        {
            ++res;
        }
    }

    return res;
}

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
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    std::string time, distance;
    std::getline(file, time);
    std::getline(file, distance);

    auto colon = time.find(':');
    time = time.substr(colon + 1);

    colon = distance.find(':');
    distance = distance.substr(colon + 1);

    time.erase(std::remove(time.begin(), time.end(), ' '), time.end());
    distance.erase(std::remove(distance.begin(), distance.end(), ' '), distance.end());

    long total_time = std::stol(time);
    long total_distance = std::stol(distance);
    long res = 0;

    for (size_t wait = 0; wait <= total_time; ++wait)
    {
        if (wait * (total_time - wait) > total_distance)
            ++res;
    }

    std::cout << res << std::endl;
    return 0;
}