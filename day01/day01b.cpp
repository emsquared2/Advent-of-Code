#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

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

    std::unordered_map<int, int> right_count;
    for (int num : right)
    {
        right_count[num]++;
    }

    int similarity_score = 0;
    for (int num : left)
    {
        similarity_score += num * right_count[num];
    }

    std::cout << "The similarity score is " << similarity_score << "! " << std::endl;

    return 0;
}