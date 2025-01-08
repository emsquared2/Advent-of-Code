#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <string>

using namespace std;

map<string, set<string>> connections;

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

        connections[a].insert(b);
        connections[b].insert(a);
    }

    return true;
}

// Recursive function to find the largest clique
void find_clique(vector<string> &current_clique, vector<string> &remaining_nodes, vector<string> &best_clique)
{
    if (remaining_nodes.empty())
    {
        if (current_clique.size() > best_clique.size())
        {
            best_clique = current_clique; // Update the best clique
        }
        return;
    }

    for (size_t i = 0; i < remaining_nodes.size(); ++i)
    {
        string node = remaining_nodes[i];
        current_clique.push_back(node);

        vector<string> new_remaining_nodes;
        for (size_t j = i + 1; j < remaining_nodes.size(); ++j)
        {
            if (connections[node].find(remaining_nodes[j]) != connections[node].end())
            {
                new_remaining_nodes.push_back(remaining_nodes[j]);
            }
        }

        find_clique(current_clique, new_remaining_nodes, best_clique);
        current_clique.pop_back(); // Backtrack
    }
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

    vector<string> all_nodes;
    for (const auto &[node, _] : connections)
    {
        all_nodes.push_back(node);
    }

    vector<string> current_clique, best_clique;
    find_clique(current_clique, all_nodes, best_clique);

    // Sort the best clique alphabetically and generate the password
    sort(best_clique.begin(), best_clique.end());
    string password = "";
    for (size_t i = 0; i < best_clique.size(); ++i)
    {
        password += best_clique[i];
        if (i != best_clique.size() - 1)
            password += ",";
    }

    cout << "The password to the LAN party is: " << password << endl;
    return 0;
}