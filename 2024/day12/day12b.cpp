#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

vector<vector<char>> grid;
vector<vector<char>> grid_copy;

vector<int> dR = {-1, 0, 1, 0}; // UP, RIGHT, DOWN, LEFT
vector<int> dC = {0, 1, 0, -1};

bool read_input_to_2d_vector(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<char> row;
        for (const char c : line)
            row.push_back(c);
        grid.push_back(row);
        grid_copy.push_back(row);
    }

    file.close();
    return true;
}

bool inbounds(int row, int col)
{
    return row >= 0 && row < grid.size() && col >= 0 && col < grid[row].size();
}

int sides(int row, int col)
{
    int plant_sides = 0;
    char plant = grid[row][col];
    for (int i = 0; i < 4; i++)
    {
        int newR = row + dR[i];
        int newC = col + dC[i];
        if (!inbounds(newR, newC) || grid_copy[newR][newC] != plant)
        {
            int newR_90CC = row + dR[(i - 1 + 4) % 4];
            int newC_90CC = col + dC[(i - 1 + 4) % 4];
            bool isBeginEdge = !inbounds(newR_90CC, newC_90CC) || grid_copy[newR_90CC][newC_90CC] != plant;

            int newR_Corner = newR + dR[(i - 1 + 4) % 4];
            int newC_Corner = newC + dC[(i - 1 + 4) % 4];
            bool isConcaveBeginEdge = inbounds(newR_Corner, newC_Corner) && grid_copy[newR_Corner][newC_Corner] == plant;

            if (isBeginEdge || isConcaveBeginEdge)
            {
                plant_sides++;
            }
        }
    }
    return plant_sides;
}

int fence_region(int startRow, int startCol)
{
    int area = 1;
    int region_sides = sides(startRow, startCol);

    char plant = grid[startRow][startCol];
    queue<pair<int, int>> q;
    q.push({startRow, startCol});
    grid[startRow][startCol] = '#';

    while (!q.empty())
    {
        auto [row, col] = q.front();
        q.pop();

        for (int i = 0; i < 4; i++)
        {
            int newR = row + dR[i];
            int newC = col + dC[i];

            if (inbounds(newR, newC) && grid[newR][newC] == plant)
            {
                area++;
                region_sides += sides(newR, newC);
                grid[newR][newC] = '#';
                q.push({newR, newC});
            }
        }
    }

    return area * region_sides;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    if (!read_input_to_2d_vector(argv[1]))
        return 1;

    int total_price = 0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] != '#')
            {
                total_price += fence_region(i, j);
            }
        }
    }

    std::cout << "The total price of fencing all regions is " << total_price << "." << std::endl;

    return 0;
}
