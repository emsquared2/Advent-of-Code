#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

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

    std::stringstream timeS(time);
    std::stringstream distanceS(distance);

    std::vector<int> times, distances;

    int t, d;
    while (timeS >> t && distanceS >> d)
    {
        times.push_back(t);
        distances.push_back(d);
    }

    long res = 1;

    for (int i = 0; i < times.size(); ++i)
    {
        res *= travel(times[i], distances[i]);
    }

    std::cout << res << std::endl;
    return 0;
}