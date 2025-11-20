#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

int pattern_occurrences(const std::string &str, std::regex &pattern)
{
    auto lineBegin = str.cbegin();
    auto lineEnd = str.cend();
    std::smatch match;

    int occurrences = 0;

    while (std::regex_search(lineBegin, lineEnd, match, pattern))
    {
        occurrences++;
        lineBegin = match.suffix().first;
    }
    return occurrences;
}

std::string get_top_left_to_bottom_right_diagonal(const std::vector<std::string> &letters, int start_row, int start_col)
{
    std::string diagonal;
    int rows = letters.size();
    int cols = letters[0].size();

    while (start_row < rows && start_col < cols)
    {
        diagonal += letters[start_row][start_col];
        start_row++;
        start_col++;
    }
    return diagonal;
}

std::string get_bottom_left_to_top_right_diagonal(const std::vector<std::string> &letters, int start_row, int start_col)
{
    std::string diagonal;
    int rows = letters.size();
    int cols = letters[0].size();

    while (start_row >= 0 && start_col < cols)
    {
        diagonal += letters[start_row][start_col];
        start_row--;
        start_col++;
    }
    return diagonal;
}

int count_pattern_in_reversed_and_normal(const std::string &str, std::regex &pattern)
{
    int occurrences = pattern_occurrences(str, pattern);
    std::string reversed_str = str;
    std::reverse(reversed_str.begin(), reversed_str.end());
    occurrences += pattern_occurrences(reversed_str, pattern);
    return occurrences;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return 1;
    }

    std::vector<std::string> letters;
    std::regex pattern(R"(XMAS)");
    std::string line;

    int occurrences = 0;

    while (std::getline(file, line))
    {
        letters.push_back(line);
        occurrences += count_pattern_in_reversed_and_normal(line, pattern);
    }

    int total_rows = letters.size();
    int total_columns = letters[0].size();

    // Count occurrences in columns
    for (int i = 0; i < total_columns; ++i)
    {
        std::string column;
        for (int j = 0; j < total_rows; ++j)
        {
            column += letters[j][i];
        }
        occurrences += count_pattern_in_reversed_and_normal(column, pattern);
    }

    // Count occurrences in diagonals (top-left to bottom-right)
    for (int i = 0; i < total_rows; ++i)
    {
        std::string diagonal = get_top_left_to_bottom_right_diagonal(letters, i, 0);
        occurrences += count_pattern_in_reversed_and_normal(diagonal, pattern);
    }

    for (int i = 1; i < total_columns; ++i)
    {
        std::string diagonal = get_top_left_to_bottom_right_diagonal(letters, 0, i);
        occurrences += count_pattern_in_reversed_and_normal(diagonal, pattern);
    }

    // Count occurrences in diagonals (bottom-left to top-right)
    for (int i = 0; i < total_rows; ++i)
    {
        std::string diagonal = get_bottom_left_to_top_right_diagonal(letters, i, 0);
        occurrences += count_pattern_in_reversed_and_normal(diagonal, pattern);
    }

    for (int i = 1; i < total_columns; ++i)
    {
        std::string diagonal = get_bottom_left_to_top_right_diagonal(letters, total_rows - 1, i);
        occurrences += count_pattern_in_reversed_and_normal(diagonal, pattern);
    }

    file.close();

    std::cout << "XMAS appeared " << occurrences << " times!" << std::endl;

    return 0;
}
