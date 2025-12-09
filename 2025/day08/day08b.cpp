#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

struct Point
{
    int x, y, z;
};

long long dist2(const Point &a, const Point &b)
{
    long long dx = (long long)a.x - b.x;
    long long dy = (long long)a.y - b.y;
    long long dz = (long long)a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

struct DSU
{
    std::vector<int> parent;
    std::vector<int> rank;
    int comps; // number of components

    DSU(int n) : parent(n), rank(n, 1), comps(n)
    {
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int find(int x)
    {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    // returns true if a and b were in different components and are now merged
    bool unite(int a, int b)
    {
        a = find(a);
        b = find(b);
        if (a == b)
            return false;

        if (rank[a] < rank[b])
            std::swap(a, b);

        parent[b] = a;
        if (rank[a] == rank[b])
            rank[a]++;

        --comps;
        return true;
    }
};

struct Edge
{
    long long w; // squared distance between point i and point j
    int i;       // index of first point
    int j;       // index of second point
};

// max-heap: largest distance first
struct EdgeCmp
{
    bool operator()(const Edge &a, const Edge &b) const
    {
        return a.w < b.w; // largest w first
    }
};

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

    // Read points
    std::vector<Point> points;
    int x, y, z;
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        char comma1, comma2;
        if (ss >> x >> comma1 >> y >> comma2 >> z)
            points.push_back({x, y, z});
    }

    int n = points.size();

    // Build all edges (i<j)
    std::vector<Edge> edges;
    edges.reserve((size_t)n * (n - 1) / 2);
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            edges.push_back(Edge{dist2(points[i], points[j]), i, j});
        }
    }

    // Sort ascending by squared distance
    std::sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b)
              { return a.w < b.w; });

    // Kruskal: unite edges until all nodes are in one component.
    DSU dsu(n);
    long long ans = 0;
    for (const Edge &e : edges)
    {
        if (dsu.unite(e.i, e.j))
        {
            // this edge merged two components
            // if this merge produced a single component, it's the last one we need
            if (dsu.comps == 1)
            {
                // multiply X coordinates of the two junction boxes
                long long xi = points[e.i].x;
                long long xj = points[e.j].x;
                ans = xi * xj;
                break;
            }
        }
    }

    std::cout << ans << "\n";
    return 0;
}
