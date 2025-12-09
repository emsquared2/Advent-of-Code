#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
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
    std::vector<int> size;

    DSU(int n) : parent(n), size(n, 1)
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

    void unite(int a, int b)
    {
        a = find(a);
        b = find(b);
        if (a == b)
            return;

        if (size[a] < size[b])
            std::swap(a, b);

        parent[b] = a;
        size[a] += size[b];
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

    // ----------- READ POINTS -----------
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

    // ----------- PRIORITY QUEUE FOR 1000 SMALLEST PAIRS -----------
    std::priority_queue<Edge, std::vector<Edge>, EdgeCmp> maxHeap;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            long long d = dist2(points[i], points[j]);
            if (maxHeap.size() < 1000)
            {
                maxHeap.push({d, i, j});
            }
            else if (d < maxHeap.top().w)
            {
                maxHeap.pop();
                maxHeap.push({d, i, j});
            }
        }
    }

    // ----------- UNION-FIND CONNECTIONS -----------
    DSU dsu(n);

    // Extract edges
    std::vector<Edge> edges;
    edges.reserve(1000);
    while (!maxHeap.empty())
    {
        edges.push_back(maxHeap.top());
        maxHeap.pop();
    }

    for (const auto &edge : edges)
    {
        dsu.unite(edge.i, edge.j);
    }

    // ----------- FIND COMPONENT SIZES -----------
    std::vector<int> compsizes;
    for (int i = 0; i < n; i++)
    {
        if (dsu.find(i) == i)
            compsizes.push_back(dsu.size[i]);
    }
    std::sort(compsizes.rbegin(), compsizes.rend());

    long long ans = 1LL * compsizes[0] * compsizes[1] * compsizes[2];

    std::cout << ans << "\n";

    return 0;
}
