#include <fstream>
#include <iostream>
#include <list>

bool read_input(const std::string &filepath, std::list<long> &stones)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return false;
    }

    long stone;
    while (file >> stone)
    {
        stones.push_back(stone);
    }

    file.close();
    return true;
}

void blink(std::list<long> &stones)
{
    for (std::list<long>::iterator it = stones.begin(); it != stones.end(); ++it)
    {
        std::string stone_str = std::to_string(*it);

        if (*it == 0)
        {
            *it = 1;
        }
        else if (stone_str.size() % 2 == 0)
        {
            int middle = stone_str.size() / 2;
            long left = std::stol(stone_str.substr(0, middle));
            long right = std::stol(stone_str.substr(middle, stone_str.size()));

            *it = left;
            auto nextIt = std::next(it);

            stones.insert(nextIt, right);
            ++it;
        }
        else
        {
            *it *= 2024;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::list<long> stones;

    if (!read_input(argv[1], stones))
        return 1;

    for (int i = 0; i < 25; ++i)
    {
        blink(stones);
        // for (const auto &stone : stones)
        //     std::cout << stone << " ";
        // std::cout << std::endl;
    }

    std::cout << "There are " << stones.size() << " stones after 25 blinks" << std::endl;
}
