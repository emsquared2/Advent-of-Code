#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>

// Function to read input from the file and parse it into the required structures
bool read_input(std::unordered_map<int, std::vector<int>> &reversed_ordering_rules,
                std::vector<std::vector<int>> &page_numbers,
                const std::string &filepath)
{

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return false;
    }

    std::string line;
    bool second_part = false; // Flag to switch between the two parts of the input file

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            second_part = true; // Switch to the second part (page numbers)
            continue;           // Skip the empty line
        }

        if (!second_part)
        {
            // Parse the first part: rules in the form X|Y (X must be before Y)
            int delimiter_pos = line.find('|');
            if (delimiter_pos != std::string::npos)
            {
                int page_x = std::stoi(line.substr(0, delimiter_pos));
                int page_y = std::stoi(line.substr(delimiter_pos + 1));
                reversed_ordering_rules[page_y].push_back(page_x); // Store the ordering rule: Y must come after X
            }
        }
        else
        {
            // Parse the second part: a list of pages to be printed for each update
            std::vector<int> page_list;
            int start_pos = 0, end_pos = 0;
            while ((end_pos = line.find(',', start_pos)) != std::string::npos)
            {
                page_list.push_back(std::stoi(line.substr(start_pos, end_pos - start_pos)));
                start_pos = end_pos + 1;
            }
            // Add the last number if there are no commas in the last segment
            if (start_pos < line.size())
            {
                page_list.push_back(std::stoi(line.substr(start_pos)));
            }

            page_numbers.push_back(page_list); // Add the current page list to the updates
        }
    }

    file.close();
    return true;
}

// Function to validate the page ordering
bool valid_page_ordering(const std::unordered_map<int, std::vector<int>> &reversed_ordering_rules,
                         const std::vector<int> &updates)
{

    int total_updates = updates.size();
    if (total_updates == 0)
        return false; // If there are no updates, return false

    // Track which pages have already been placed correctly
    std::unordered_set<int> pages_placed;

    // Go through each page in the update list and check if it can be placed
    for (const int &current_page : updates)
    {
        // Check if the current page has any predecessors in the reversed_ordering_rules
        if (reversed_ordering_rules.find(current_page) != reversed_ordering_rules.end())
        {
            // Loop through all the pages that must be placed before the current page
            for (const auto &required_predecessor : reversed_ordering_rules.at(current_page))
            {
                // Only check the predecessors that are part of the current update list
                if (std::find(updates.begin(), updates.end(), required_predecessor) != updates.end())
                {
                    // If the required predecessor isn't placed yet, the order is invalid
                    if (pages_placed.find(required_predecessor) == pages_placed.end())
                    {
                        return false; // Invalid order
                    }
                }
            }
        }
        // Mark the current page as placed in the correct order
        pages_placed.insert(current_page);
    }

    return true; // Return true if the order is valid
}

void fix_order(const std::unordered_map<int, std::vector<int>> &reversed_ordering_rules, std::vector<int> &page_list)
{
    // Create adjacency list and in-degrees for the current page_list
    std::unordered_map<int, std::vector<int>> adjacency_list;
    std::unordered_map<int, int> in_degree;

    // Initialize in-degrees for all pages in the page_list
    for (int page : page_list)
    {
        in_degree[page] = 0;
    }

    // Build the graph for the subset of pages in the current page_list
    for (int page : page_list)
    {
        if (reversed_ordering_rules.find(page) != reversed_ordering_rules.end())
        {
            for (int prerequisite : reversed_ordering_rules.at(page))
            {
                if (std::find(page_list.begin(), page_list.end(), prerequisite) != page_list.end())
                {
                    adjacency_list[prerequisite].push_back(page);
                    in_degree[page]++;
                }
            }
        }
    }

    // Perform a topological sort using Kahn's Algorithm
    std::vector<int> sorted_pages;
    std::queue<int> zero_in_degree;

    // Start with pages that have zero in-degree
    for (auto &entry : in_degree)
    {
        if (entry.second == 0)
        {
            zero_in_degree.push(entry.first);
        }
    }

    while (!zero_in_degree.empty())
    {
        int current = zero_in_degree.front();
        zero_in_degree.pop();
        sorted_pages.push_back(current);

        // Reduce in-degree of neighbors
        if (adjacency_list.find(current) != adjacency_list.end())
        {
            for (int neighbor : adjacency_list[current])
            {
                in_degree[neighbor]--;
                if (in_degree[neighbor] == 0)
                {
                    zero_in_degree.push(neighbor);
                }
            }
        }
    }

    // Replace the page_list with the sorted order
    page_list = sorted_pages;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::unordered_map<int, std::vector<int>> reversed_ordering_rules; // Map to store the reversed ordering rules (page Y comes after X)
    std::vector<std::vector<int>> page_numbers;                        // Vector to store lists of page numbers to be printed in each update

    // Read the input file and populate the data structures
    if (!read_input(reversed_ordering_rules, page_numbers, argv[1]))
    {
        return 1; // If reading the input failed, exit the program
    }

    int total_middle_page_number = 0;

    // Process each list of page numbers in the updates
    for (auto &page_list : page_numbers)
    {
        if (!valid_page_ordering(reversed_ordering_rules, page_list))
        {
            fix_order(reversed_ordering_rules, page_list);
            int total_updates = page_list.size();
            if (total_updates == 0)
                continue;
            // Calculate the index of the middle page (based on zero indexing)
            int middle_index = (total_updates - 1) / 2;
            int middle_page = page_list[middle_index];
            total_middle_page_number += middle_page;
        }
    }

    std::cout << "The total middle page number from corrected updates is "
              << total_middle_page_number << std::endl;
    return 0;
}
