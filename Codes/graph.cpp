#include "graph.hpp"
#include "display_graph.hpp"
#include <queue>
#include <stack>
#include <thread>
#include <chrono>
#include <functional>

// Custom comparator for priority queue (used in Dijkstra)
struct ComparePairs {
    bool operator()(const std::pair<int, sf::Vector2i>& a, const std::pair<int, sf::Vector2i>& b) const {
        return a.first > b.first; // Compare based on distance (Dijkstra's priority queue)
    }
};

// BFS Implementation with correct termination and no path paving
void runBFS(std::vector<std::vector<CellType>>& grid, sf::Vector2i startPos, sf::Vector2i endPos, sf::RenderWindow &window) {
    std::queue<sf::Vector2i> q;
    std::vector<std::vector<bool>> visited(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false));

    q.push(startPos);
    visited[startPos.x][startPos.y] = true;

    while (!q.empty()) {
        sf::Vector2i current = q.front();
        q.pop();

        // Check if we reached the end
        if (current == endPos) {
            return; // Terminate BFS once end is reached
        }

        // Visualization step
        if (current != startPos && current != endPos) {
            grid[current.x][current.y] = PATH; // Temporary marking for visualization, not final path
        }

        window.clear();
        drawGrid(grid, window);
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));

        // Explore neighbors
        for (auto& neighbor : getNeighbors(current.x, current.y)) {
            if (neighbor.x >= 0 && neighbor.x < GRID_WIDTH && neighbor.y >= 0 && neighbor.y < GRID_HEIGHT &&
                grid[neighbor.x][neighbor.y] != BLOCK && !visited[neighbor.x][neighbor.y]) {
                q.push(neighbor);
                visited[neighbor.x][neighbor.y] = true;
            }
        }
    }
}

// DFS Implementation with correct termination and no path paving
void runDFS(std::vector<std::vector<CellType>>& grid, sf::Vector2i startPos, sf::Vector2i endPos, sf::RenderWindow &window) {
    std::stack<sf::Vector2i> s;
    std::vector<std::vector<bool>> visited(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false));

    s.push(startPos);
    visited[startPos.x][startPos.y] = true;

    while (!s.empty()) {
        sf::Vector2i current = s.top();
        s.pop();

        // Check if we reached the end
        if (current == endPos) {
            return; // Terminate DFS once end is reached
        }

        // Visualization step
        if (current != startPos && current != endPos) {
            grid[current.x][current.y] = PATH; // Temporary marking for visualization, not final path
        }

        window.clear();
        drawGrid(grid, window);
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));

        // Explore neighbors
        for (auto& neighbor : getNeighbors(current.x, current.y)) {
            if (neighbor.x >= 0 && neighbor.x < GRID_WIDTH && neighbor.y >= 0 && neighbor.y < GRID_HEIGHT &&
                grid[neighbor.x][neighbor.y] != BLOCK && !visited[neighbor.x][neighbor.y]) {
                s.push(neighbor);
                visited[neighbor.x][neighbor.y] = true;
            }
        }
    }
}

// Dijkstra Implementation with final path visualization
void runDijkstra(std::vector<std::vector<CellType>>& grid, sf::Vector2i startPos, sf::Vector2i endPos, sf::RenderWindow &window) {
    std::priority_queue<std::pair<int, sf::Vector2i>, std::vector<std::pair<int, sf::Vector2i>>, ComparePairs> pq;
    std::vector<std::vector<int>> distances(GRID_WIDTH, std::vector<int>(GRID_HEIGHT, INT_MAX));
    std::vector<std::vector<sf::Vector2i>> previous(GRID_WIDTH, std::vector<sf::Vector2i>(GRID_HEIGHT, {-1, -1}));
    std::vector<std::vector<bool>> visited(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false));

    pq.push({0, startPos});
    distances[startPos.x][startPos.y] = 0;

    while (!pq.empty()) {
        std::pair<int, sf::Vector2i> current_pair = pq.top();
        sf::Vector2i current = current_pair.second;
        pq.pop();

        if (visited[current.x][current.y]) {
            continue;
        }

        visited[current.x][current.y] = true;

        if (current != startPos && current != endPos) {
            grid[current.x][current.y] = PATH;  // Pave path during exploration for Dijkstra
        }

        window.clear();
        drawGrid(grid, window);
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));

        if (current == endPos) {
            // Backtrack the path and visualize it
            sf::Vector2i backtrack = endPos;
            while (backtrack != startPos) {
                if (backtrack != startPos && backtrack != endPos) {
                    grid[backtrack.x][backtrack.y] = END; // Final path in red
                }
                backtrack = previous[backtrack.x][backtrack.y];
                
                window.clear();
                drawGrid(grid, window);
                window.display();
                std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
            }
            return;
        }

        // Explore neighbors
        for (auto& neighbor : getNeighbors(current.x, current.y)) {
            if (neighbor.x >= 0 && neighbor.x < GRID_WIDTH && neighbor.y >= 0 && neighbor.y < GRID_HEIGHT &&
                grid[neighbor.x][neighbor.y] != BLOCK && !visited[neighbor.x][neighbor.y]) {
                int newDist = distances[current.x][current.y] + 1;
                if (newDist < distances[neighbor.x][neighbor.y]) {
                    distances[neighbor.x][neighbor.y] = newDist;
                    previous[neighbor.x][neighbor.y] = current;
                    pq.push({newDist, neighbor});
                }
            }
        }
    }
}

// Function to get neighbors of a node
std::vector<sf::Vector2i> getNeighbors(int x, int y) {
    std::vector<sf::Vector2i> neighbors = {
        {x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1}
    };
    return neighbors;
}