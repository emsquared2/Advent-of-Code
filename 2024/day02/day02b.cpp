#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

// Function to check if a report is safe
auto is_safe = [](const std::vector<int> &report) -> bool
{
    if (report.size() < 2)
    {
        return true; // A report with less than 2 levels is trivially safe
    }

    bool desc = report[0] > report[1]; // Determine if the report is descending
    for (int j = 1; j < report.size(); ++j)
    {
        int prev = report[j - 1];
        int current = report[j];
        int diff = prev - current;
        bool legal_diff = std::abs(diff) >= 1 && std::abs(diff) <= 3;

        if (!legal_diff || (desc && diff < 0) || (!desc && diff > 0))
        {
            return false;
        }
    }
    return true;
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
        std::cerr << "Error: Could not open the file." << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> reports; // 2D vector to store reports and levels
    std::vector<int> currentReport;        // Vector to store levels of a single report
    int level;

    while (file >> level)
    {
        currentReport.push_back(level);

        if (file.peek() == '\n' || file.eof())
        {
            reports.push_back(currentReport); // Store the completed report
            currentReport.clear();            // Clear for the next report
        }
    }

    int safe_reports = 0;

    // Process each report
    for (const auto &report : reports)
    {
        bool safe = false;

        for (int i = 0; i < report.size(); ++i)
        {
            // Create a modified report by skipping the ith level
            std::vector<int> modifiedReport = report;
            modifiedReport.erase(modifiedReport.begin() + i);

            if (is_safe(modifiedReport))
            {
                safe = true;
                break; // No need to check further if removing this level makes the report safe
            }
        }

        if (safe || is_safe(report))
        {
            ++safe_reports;
        }
    }

    std::cout << "Total safe reports (including those made safe by removing a level): " << safe_reports << "!" << std::endl;

    return 0;
}