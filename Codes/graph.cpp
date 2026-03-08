#include "graph.hpp"

#include <algorithm>
#include <climits>
#include <queue>
#include <stack>

namespace {

GridPoint invalidPoint() {
    return GridPoint{-1, -1};
}

bool isBlock(const Grid& grid, const GridPoint& p) {
    return grid[p.x][p.y] == CellType::Block;
}

std::vector<GridPoint> reconstructPath(
    const std::vector<std::vector<GridPoint>>& previous,
    GridPoint start,
    GridPoint end
) {
    std::vector<GridPoint> path;

    if (start == end) {
        path.push_back(start);
        return path;
    }

    GridPoint cursor = end;
    while (cursor != invalidPoint()) {
        path.push_back(cursor);
        if (cursor == start) {
            break;
        }
        cursor = previous[cursor.x][cursor.y];
    }

    if (path.empty() || path.back() != start) {
        return {};
    }

    std::reverse(path.begin(), path.end());
    return path;
}

GraphTraversalResult runBfs(const Grid& grid, GridPoint start, GridPoint end) {
    GraphTraversalResult result;

    const int width = static_cast<int>(grid.size());
    const int height = static_cast<int>(grid[0].size());
    std::vector<std::vector<bool>> visited(width, std::vector<bool>(height, false));
    std::vector<std::vector<GridPoint>> previous(width, std::vector<GridPoint>(height, invalidPoint()));
    std::queue<GridPoint> q;

    q.push(start);
    visited[start.x][start.y] = true;

    while (!q.empty()) {
        GridPoint current = q.front();
        q.pop();
        result.visitOrder.push_back(current);

        if (current == end) {
            result.found = true;
            result.path = reconstructPath(previous, start, end);
            return result;
        }

        for (const GridPoint& neighbor : getNeighbors(current)) {
            if (!isInsideGrid(grid, neighbor) || isBlock(grid, neighbor) || visited[neighbor.x][neighbor.y]) {
                continue;
            }

            visited[neighbor.x][neighbor.y] = true;
            previous[neighbor.x][neighbor.y] = current;
            q.push(neighbor);
        }
    }

    return result;
}

GraphTraversalResult runDfs(const Grid& grid, GridPoint start, GridPoint end) {
    GraphTraversalResult result;

    const int width = static_cast<int>(grid.size());
    const int height = static_cast<int>(grid[0].size());
    std::vector<std::vector<bool>> visited(width, std::vector<bool>(height, false));
    std::vector<std::vector<GridPoint>> previous(width, std::vector<GridPoint>(height, invalidPoint()));
    std::stack<GridPoint> st;

    st.push(start);
    visited[start.x][start.y] = true;

    while (!st.empty()) {
        GridPoint current = st.top();
        st.pop();
        result.visitOrder.push_back(current);

        if (current == end) {
            result.found = true;
            result.path = reconstructPath(previous, start, end);
            return result;
        }

        std::vector<GridPoint> neighbors = getNeighbors(current);
        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
            const GridPoint& neighbor = *it;
            if (!isInsideGrid(grid, neighbor) || isBlock(grid, neighbor) || visited[neighbor.x][neighbor.y]) {
                continue;
            }

            visited[neighbor.x][neighbor.y] = true;
            previous[neighbor.x][neighbor.y] = current;
            st.push(neighbor);
        }
    }

    return result;
}

struct QueueNode {
    int dist;
    GridPoint point;
};

struct CompareQueueNode {
    bool operator()(const QueueNode& a, const QueueNode& b) const {
        return a.dist > b.dist;
    }
};

GraphTraversalResult runDijkstra(const Grid& grid, GridPoint start, GridPoint end) {
    GraphTraversalResult result;

    const int width = static_cast<int>(grid.size());
    const int height = static_cast<int>(grid[0].size());
    std::vector<std::vector<int>> distance(width, std::vector<int>(height, INT_MAX));
    std::vector<std::vector<GridPoint>> previous(width, std::vector<GridPoint>(height, invalidPoint()));
    std::vector<std::vector<bool>> visited(width, std::vector<bool>(height, false));

    std::priority_queue<QueueNode, std::vector<QueueNode>, CompareQueueNode> pq;
    distance[start.x][start.y] = 0;
    pq.push(QueueNode{0, start});

    while (!pq.empty()) {
        QueueNode node = pq.top();
        pq.pop();

        GridPoint current = node.point;
        if (visited[current.x][current.y]) {
            continue;
        }

        visited[current.x][current.y] = true;
        result.visitOrder.push_back(current);

        if (current == end) {
            result.found = true;
            result.path = reconstructPath(previous, start, end);
            return result;
        }

        for (const GridPoint& neighbor : getNeighbors(current)) {
            if (!isInsideGrid(grid, neighbor) || isBlock(grid, neighbor) || visited[neighbor.x][neighbor.y]) {
                continue;
            }

            const int newDistance = distance[current.x][current.y] + 1;
            if (newDistance < distance[neighbor.x][neighbor.y]) {
                distance[neighbor.x][neighbor.y] = newDistance;
                previous[neighbor.x][neighbor.y] = current;
                pq.push(QueueNode{newDistance, neighbor});
            }
        }
    }

    return result;
}

}  // namespace

const char* graphAlgorithmName(GraphAlgorithm algorithm) {
    switch (algorithm) {
        case GraphAlgorithm::BFS:
            return "BFS";
        case GraphAlgorithm::DFS:
            return "DFS";
        case GraphAlgorithm::Dijkstra:
            return "Dijkstra";
        default:
            return "Unknown";
    }
}

bool isInsideGrid(const Grid& grid, const GridPoint& point) {
    if (grid.empty() || grid[0].empty()) {
        return false;
    }

    return point.x >= 0 && point.y >= 0
        && point.x < static_cast<int>(grid.size())
        && point.y < static_cast<int>(grid[0].size());
}

std::vector<GridPoint> getNeighbors(const GridPoint& point) {
    return {
        GridPoint{point.x + 1, point.y},
        GridPoint{point.x - 1, point.y},
        GridPoint{point.x, point.y + 1},
        GridPoint{point.x, point.y - 1}
    };
}

GraphTraversalResult traverseGrid(const Grid& grid, GridPoint start, GridPoint end, GraphAlgorithm algorithm) {
    if (!isInsideGrid(grid, start) || !isInsideGrid(grid, end)) {
        return {};
    }

    if (isBlock(grid, start) || isBlock(grid, end)) {
        return {};
    }

    switch (algorithm) {
        case GraphAlgorithm::BFS:
            return runBfs(grid, start, end);
        case GraphAlgorithm::DFS:
            return runDfs(grid, start, end);
        case GraphAlgorithm::Dijkstra:
            return runDijkstra(grid, start, end);
        default:
            return {};
    }
}
