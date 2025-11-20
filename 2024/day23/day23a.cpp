#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <string>

using namespace std;

map<string, vector<string>> connections;

bool read_input(const string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        cerr << "Error: Could not open the file!" << endl;
        return false;
    }
    string line;
    while (getline(file, line))
    {
        string a, b;
        size_t delimiter_pos = line.find('-');

        a = line.substr(0, delimiter_pos);  // Extract part before '-'
        b = line.substr(delimiter_pos + 1); // Extract part after '-'

        connections[a].push_back(b);
        connections[b].push_back(a);
    }

    return true;
}

set<set<string>> find_triangles()
{
    set<set<string>> triangles;

    // Iterate through each computer and its connections
    for (auto &[node, neighbors] : connections)
    {
        for (size_t i = 0; i < neighbors.size(); ++i)
        {
            for (size_t j = i + 1; j < neighbors.size(); ++j)
            {
                const string &neighbor1 = neighbors[i];
                const string &neighbor2 = neighbors[j];

                // Check if neighbor1 and neighbor2 are connected
                if (find(connections[neighbor1].begin(), connections[neighbor1].end(), neighbor2) != connections[neighbor1].end())
                {
                    // Found a triangle
                    set<string> triangle = {node, neighbor1, neighbor2};
                    triangles.insert(triangle);
                }
            }
        }
    }

    return triangles;
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

    auto triangles = find_triangles();

    int count = 0;

    for (const auto &triangle : triangles)
    {
        for (const auto &computer : triangle)
        {
            if (computer[0] == 't')
            {
                count++;
                break;
            }
        }
    }

    cout << count << endl;

    return 0;
}
