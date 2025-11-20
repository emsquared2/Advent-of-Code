#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

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

    std::vector<int> left, right;
    int l, r;

    while (file >> l >> r)
    {
        left.push_back(l);
        right.push_back(r);
    }

    if (left.empty() || right.empty())
    {
        std::cerr << "Error: No data to process." << std::endl;
        return 1;
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    int distance = 0;
    for (int i = 0; i < left.size(); ++i)
        distance += std::abs(left[i] - right[i]);

    std::cout << "Total distance is " << distance << "! " << std::endl;

    return 0;
}