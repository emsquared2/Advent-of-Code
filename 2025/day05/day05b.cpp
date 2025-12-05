#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Interval
{
    long start;
    long end;
};

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

    std::string line;
    std::vector<Interval> intervals;

    // Read range lines until empty line
    while (std::getline(file, line))
    {
        if (line.empty())
            break; // blank line separates sections

        size_t dash = line.find('-');
        long startID = std::stol(line.substr(0, dash));
        long endID = std::stol(line.substr(dash + 1));

        intervals.push_back({startID, endID});
    }

    std::sort(intervals.begin(), intervals.end(),
              [](const Interval &a, const Interval &b)
              { return a.start < b.start; });

    // Merge overlapping or adjacent intervals
    std::vector<Interval> merged;
    for (const auto &interval : intervals)
    {
        if (merged.empty() || merged.back().end + 1 < interval.start)
            merged.push_back(interval);
        else
            merged.back().end = std::max(merged.back().end, interval.end);
    }

    long freshCount = 0;

    for (const auto &i : merged)
    {
        freshCount += (i.end - i.start + 1);
    }

    std::cout << freshCount << " of the available IDs are fresh. " << std::endl;
    return 0;
}