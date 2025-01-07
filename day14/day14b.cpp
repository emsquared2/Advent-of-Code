#include <fstream>
#include <iostream>
#include <vector>
#include <climits> // For INT_MAX

const int WIDTH = 101;
const int HEIGHT = 103;
const int MIDDLE_ROW = HEIGHT / 2;
const int MIDDLE_COL = WIDTH / 2;

struct Robot
{
    int x;
    int y;
    int x_v;
    int y_v;

    void print() const
    {
        std::cout << "Position --> X: " << x << ", Y: " << y << std::endl;
        std::cout << "Velocity --> X: " << x_v << ", Y: " << y_v << std::endl;
    }
};

std::vector<Robot> robots;
std::vector<std::vector<int>> tiles(HEIGHT, std::vector<int>(WIDTH, 0));
std::vector<int> quadrants(4, 0); // Holds counts for the 4 quadrants

void print_tiles()
{
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            if (tiles[i][j] == 0)
                std::cout << ".";
            else
                std::cout << "#";
        }
        std::cout << std::endl;
    }
}

bool read_input(const std::string &filepath)
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
        Robot robot;
        std::sscanf(line.c_str(), "p=%d,%d v=%d,%d", &robot.y, &robot.x, &robot.y_v, &robot.x_v);
        robots.push_back(robot);
    }
    file.close();
    return true;
}

void initialize()
{
    for (const auto &robot : robots)
    {
        tiles[robot.x][robot.y]++;
    }
}

void move()
{
    // Clear current robot positions
    for (auto &robot : robots)
    {
        tiles[robot.x][robot.y]--;
    }

    // Move robots and wrap them correctly
    for (auto &robot : robots)
    {
        robot.x = (robot.x + robot.x_v + HEIGHT) % HEIGHT;
        robot.y = (robot.y + robot.y_v + WIDTH) % WIDTH;
        tiles[robot.x][robot.y]++;
    }
}

void compute_quadrants()
{
    // Reset quadrants to 0 before recalculating
    std::fill(quadrants.begin(), quadrants.end(), 0);

    // Top-left quadrant (excluding middle row and column)
    for (int i = 0; i < MIDDLE_ROW; ++i)
        for (int j = 0; j < MIDDLE_COL; ++j)
            quadrants[0] += tiles[i][j];

    // Top-right quadrant (excluding middle row and column)
    for (int i = 0; i < MIDDLE_ROW; ++i)
        for (int j = MIDDLE_COL + 1; j < WIDTH; ++j)
            quadrants[1] += tiles[i][j];

    // Bottom-left quadrant (excluding middle row and column)
    for (int i = MIDDLE_ROW + 1; i < HEIGHT; ++i)
        for (int j = 0; j < MIDDLE_COL; ++j)
            quadrants[2] += tiles[i][j];

    // Bottom-right quadrant (excluding middle row and column)
    for (int i = MIDDLE_ROW + 1; i < HEIGHT; ++i)
        for (int j = MIDDLE_COL + 1; j < WIDTH; ++j)
            quadrants[3] += tiles[i][j];
}

int find_min_safety_factor(int max_iterations)
{
    int min_safety_factor = INT_MAX;
    int min_time_step = 0;

    for (int t = 1; t <= max_iterations; ++t)
    {
        if (t == 8160)
            print_tiles();

        move();              // Move the robots
        compute_quadrants(); // Compute robots in each quadrant

        int safety_factor = quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];

        if (safety_factor < min_safety_factor)
        {
            min_safety_factor = safety_factor;
            min_time_step = t;
        }
    }

    return min_time_step;
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

    initialize();

    int max_iterations = 10000;
    int time_step_of_tree = find_min_safety_factor(max_iterations);

    std::cout << "The robots form the Christmas tree at time step: " << time_step_of_tree << std::endl;

    return 0;
}
