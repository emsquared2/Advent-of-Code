#include <fstream>
#include <iostream>
#include <vector>
#include <string>

bool read_input(const std::string &filepath, std::string &disk_map)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line);

    if (line.empty())
    {
        std::cerr << "Error: Input file is empty!" << std::endl;
        return false;
    }

    int id = 0;

    for (int i = 0; i < line.size(); ++i)
    {
        int files = line[i] - '0';
        int free_space = line[++i] - '0';

        for (int j = 0; j < files; ++j)
            disk_map += std::to_string(id) + " ";
        for (int j = 0; j < free_space; ++j)
            disk_map += ". ";

        ++id;
    }

    file.close();
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string disk_map;
    std::vector<std::string> blocks;

    if (!read_input(argv[1], disk_map))
    {
        return 1;
    }

    int start = 0, end;

    while ((end = disk_map.find(' ', start)) != std::string::npos)
    {
        blocks.push_back(disk_map.substr(start, end - start));
        start = end + 1;
    }

    for (int i = 0, j = blocks.size() - 1; i < j;)
    {
        if (blocks[i] != ".")
        {
            ++i;
            continue;
        }

        if (blocks[j] == ".")
        {
            --j;
            continue;
        }

        blocks[i] = blocks[j];
        blocks[j] = ".";

        ++i;
        --j;
    }

    long long checksum = 0;
    for (int position = 0; position < blocks.size(); ++position)
    {
        if (blocks[position] == ".")
            break;

        int id = std::stoi(blocks[position]);
        checksum += position * id;
    }

    std::cout << "Filesystem checksum: " << checksum << std::endl;

    return 0;
}
