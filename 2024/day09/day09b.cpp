#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Function to read the input file and parse the disk map
bool read_input(const std::string &filepath, std::vector<std::string> &blocks)
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
    for (size_t i = 0; i < line.size(); ++i)
    {
        int files = line[i] - '0';
        int free_space = line[++i] - '0';

        for (int j = 0; j < files; ++j)
            blocks.push_back(std::to_string(id));
        for (int j = 0; j < free_space; ++j)
            blocks.push_back(".");

        ++id;
    }

    file.close();
    return true;
}

// Function to find the leftmost free space that can accommodate a file
int find_free_space(const std::vector<std::string> &blocks, int file_length, int file_start)
{
    int free_count = 0;
    int start_idx = -1;

    for (int i = 0; i < file_start; ++i)
    {
        if (blocks[i] == ".")
        {
            if (free_count == 0)
                start_idx = i;
            ++free_count;
            if (free_count == file_length)
                return start_idx;
        }
        else
        {
            free_count = 0;
        }
    }
    return -1; // No sufficient free space found
}

// Function to identify files and their positions
void find_files(const std::vector<std::string> &blocks, std::vector<std::tuple<int, int, int>> &files)
{
    int current_id = -1;
    int count = 0;
    int start_idx = -1;

    for (int i = 0; i < blocks.size(); ++i)
    {
        if (blocks[i] == ".")
        {
            if (current_id != -1)
            {
                files.emplace_back(current_id, start_idx, count);
                current_id = -1;
                count = 0;
            }
        }
        else
        {
            int id = std::stoi(blocks[i]);
            if (id != current_id)
            {
                if (current_id != -1)
                    files.emplace_back(current_id, start_idx, count);
                current_id = id;
                start_idx = i;
                count = 0;
            }
            ++count;
        }
    }
    if (current_id != -1)
        files.emplace_back(current_id, start_idx, count);

    // Sort files by ID in descending order
    std::sort(files.rbegin(), files.rend());
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }
    std::vector<std::string> blocks;

    if (!read_input(argv[1], blocks))
    {
        return 1;
    }

    // Identify files and their positions
    std::vector<std::tuple<int, int, int>> files; // (file_id, start_index, length)
    find_files(blocks, files);

    // Move files to the leftmost free space
    for (const auto &[file_id, start_idx, file_length] : files)
    {
        int free_idx = find_free_space(blocks, file_length, start_idx);

        if (free_idx != -1)
        {
            // Remove file from current position
            for (int i = start_idx; i < start_idx + file_length; ++i)
                blocks[i] = ".";

            // Place file in the new position
            for (int i = 0; i < file_length; ++i)
                blocks[free_idx + i] = std::to_string(file_id);
        }
    }

    // Calculate checksum
    long long checksum = 0;
    for (size_t position = 0; position < blocks.size(); ++position)
    {
        if (blocks[position] == ".")
            continue;

        int id = std::stoi(blocks[position]);
        checksum += position * id;
    }

    std::cout << "Filesystem checksum: " << checksum << std::endl;

    return 0;
}
