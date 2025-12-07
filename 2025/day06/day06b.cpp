#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

static long long column_to_number(const std::vector<std::vector<char>> &grid, int col)
{
    long long value = 0;
    bool has_digit = false;

    for (const auto &row : grid)
    {
        char c = row[col];
        if (c >= '0' && c <= '9')
        {
            has_digit = true;
            value = value * 10 + (c - '0');
        }
    }
    return has_digit ? value : -1; // -1 means "empty column"
}

static bool is_separator_column(const std::vector<std::vector<char>> &grid, int col)
{
    for (const auto &row : grid)
        if (row[col] != ' ')
            return false;
    return true;
}

static long long apply_operation(char op, const std::vector<long long> &nums)
{
    if (nums.empty())
        return 0;

    long long result = (op == '+') ? 0 : 1;
    for (long long v : nums)
    {
        if (op == '+')
            result += v;
        else
            result *= v;
    }
    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;

    // Read all non-empty lines
    while (std::getline(file, line))
        if (!line.empty())
            lines.push_back(line);

    if (lines.empty())
    {
        std::cerr << "Error: Empty file.\n";
        return 1;
    }

    // Last line = operators
    std::string ops_line = lines.back();
    lines.pop_back();

    // Determine width
    size_t max_width = ops_line.size();
    for (auto &l : lines)
        max_width = std::max(max_width, l.size());

    // Build padded grid
    std::vector<std::vector<char>> grid(lines.size(), std::vector<char>(max_width, ' '));
    for (size_t r = 0; r < lines.size(); r++)
        for (size_t c = 0; c < lines[r].size(); c++)
            grid[r][c] = lines[r][c];

    // padded ops
    std::vector<char> ops(max_width, ' ');
    for (size_t i = 0; i < ops_line.size(); i++)
        ops[i] = ops_line[i];

    long long grand_total = 0;

    std::vector<long long> numbers; // numbers for current problem
    char current_op = ' ';

    // Scan right→left
    for (int col = max_width - 1; col >= 0; --col)
    {
        bool sep = is_separator_column(grid, col);
        char op = ops[col];

        // When we hit a full-space separator end of a problem
        if (sep && op == ' ')
        {
            if (current_op != ' ' && !numbers.empty())
                grand_total += apply_operation(current_op, numbers);

            numbers.clear();
            current_op = ' ';
            continue;
        }

        // Determine column number
        long long num = column_to_number(grid, col);
        if (num != -1)
            numbers.push_back(num);

        // Operator found (overwrites previous; correct since bottom row)
        if (op == '+' || op == '*')
            current_op = op;
    }

    // Leftmost unprocessed problem
    if (current_op != ' ' && !numbers.empty())
        grand_total += apply_operation(current_op, numbers);

    std::cout << "Grand total = " << grand_total << "\n";
    return 0;
}
