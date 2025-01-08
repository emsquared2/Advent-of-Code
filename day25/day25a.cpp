#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

vector<vector<string>> locks;
vector<vector<string>> keys;

bool is_lock(const string &line)
{
    return line[0] == '#';
}

bool read_input(const string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        cerr << "Error: Could not open the file " << filepath << endl;
        return false;
    }

    string line;
    vector<string> schematic;

    while (getline(file, line))
    {
        if (line.empty())
        {
            if (!schematic.empty())
            {
                if (is_lock(schematic[0]))
                {
                    locks.push_back(schematic);
                }
                else
                {
                    keys.push_back(schematic);
                }
                schematic.clear();
            }
        }
        else
        {
            schematic.push_back(line);
        }
    }

    // Handle the last schematic after the loop ends
    if (!schematic.empty())
    {
        if (is_lock(schematic[0]))
        {
            locks.push_back(schematic);
        }
        else
        {
            keys.push_back(schematic);
        }
    }

    return true;
}

bool is_pair(const vector<string> &lock, const vector<string> &key)
{
    int rows = lock.size();
    int cols = lock[0].size();

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            if (key[row][col] == '#' && lock[row][col] == '#')
            {
                return false;
            }
        }
    }
    return true;
}

int count_unique_pairs()
{
    int unique_pairs = 0;

    for (const auto &lock : locks)
    {
        for (const auto &key : keys)
        {
            if (is_pair(lock, key))
            {
                unique_pairs++;
            }
        }
    }

    return unique_pairs;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    if (!read_input(argv[1]))
        return 1;

    int unique_pairs = count_unique_pairs();
    cout << unique_pairs << endl;

    return 0;
}
