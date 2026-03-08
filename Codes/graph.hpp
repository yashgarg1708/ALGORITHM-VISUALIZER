#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

enum class CellType {
    Empty,
    Start,
    End,
    Block,
    Visited,
    Path
};

struct GridPoint {
    int x = 0;
    int y = 0;
};

inline bool operator==(const GridPoint& a, const GridPoint& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const GridPoint& a, const GridPoint& b) {
    return !(a == b);
}

enum class GraphAlgorithm {
    BFS = 1,
    DFS = 2,
    Dijkstra = 3
};

using Grid = std::vector<std::vector<CellType>>;

struct GraphTraversalResult {
    std::vector<GridPoint> visitOrder;
    std::vector<GridPoint> path;
    bool found = false;
};

const char* graphAlgorithmName(GraphAlgorithm algorithm);
bool isInsideGrid(const Grid& grid, const GridPoint& point);
std::vector<GridPoint> getNeighbors(const GridPoint& point);
GraphTraversalResult traverseGrid(const Grid& grid, GridPoint start, GridPoint end, GraphAlgorithm algorithm);

#endif
