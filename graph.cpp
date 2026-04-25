#include "graph.hpp"
#include "display_graph.hpp"
#include <deque>
#include <queue>
#include <thread>
#include <chrono>
#include <limits>

void runSearch(std::vector<std::vector<CellType>>& grid, sf::Vector2i start, sf::Vector2i end, sf::RenderWindow& window, bool bfs) {
    std::deque<sf::Vector2i> q;
    std::vector<std::vector<bool>> visited(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false));
    q.push_back(start);
    visited[start.x][start.y] = true;
    while (!q.empty()) {
        sf::Vector2i cur = bfs ? q.front() : q.back();
        if (bfs) q.pop_front();
        else q.pop_back();
        if (cur == end) return;
        if (cur != start && cur != end) grid[cur.x][cur.y] = PATH;
        window.clear(); drawGrid(grid, window); window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
        for (auto& nb : getNeighbors(cur.x, cur.y))
            if (nb.x >= 0 && nb.x < GRID_WIDTH && nb.y >= 0 && nb.y < GRID_HEIGHT &&
                grid[nb.x][nb.y] != BLOCK && !visited[nb.x][nb.y]) {
                q.push_back(nb);
                visited[nb.x][nb.y] = true;
            }
    }
}

void runDijkstra(std::vector<std::vector<CellType>>& grid, sf::Vector2i startPos, sf::Vector2i endPos, sf::RenderWindow& window) {
    using P = std::pair<int, sf::Vector2i>;
    auto cmp = [](const P& a, const P& b){ return a.first > b.first; };
    std::priority_queue<P, std::vector<P>, decltype(cmp)> pq(cmp);
    std::vector<std::vector<int>> dist(GRID_WIDTH, std::vector<int>(GRID_HEIGHT, std::numeric_limits<int>::max()));
    std::vector<std::vector<sf::Vector2i>> prev(GRID_WIDTH, std::vector<sf::Vector2i>(GRID_HEIGHT, {-1, -1}));
    std::vector<std::vector<bool>> visited(GRID_WIDTH, std::vector<bool>(GRID_HEIGHT, false));

    pq.push({0, startPos});
    dist[startPos.x][startPos.y] = 0;

    while (!pq.empty()) {
        sf::Vector2i cur = pq.top().second;
        pq.pop();
        if (visited[cur.x][cur.y]) continue;
        visited[cur.x][cur.y] = true;
        if (cur != startPos && cur != endPos) grid[cur.x][cur.y] = PATH;
        window.clear(); drawGrid(grid, window); window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
        if (cur == endPos) {
            sf::Vector2i bt = endPos;
            while (bt != startPos) {
                if (bt.x < 0 || bt.y < 0) return;
                if (bt != startPos && bt != endPos) grid[bt.x][bt.y] = FINAL_PATH;
                bt = prev[bt.x][bt.y];
                window.clear(); drawGrid(grid, window); window.display();
                std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
            }
            return;
        }
        for (auto& nb : getNeighbors(cur.x, cur.y)) {
            if (nb.x >= 0 && nb.x < GRID_WIDTH && nb.y >= 0 && nb.y < GRID_HEIGHT &&
                grid[nb.x][nb.y] != BLOCK && !visited[nb.x][nb.y]) {
                int d = dist[cur.x][cur.y] + 1;
                if (d < dist[nb.x][nb.y]) {
                    dist[nb.x][nb.y] = d;
                    prev[nb.x][nb.y] = cur;
                    pq.push({d, nb});
                }
            }
        }
    }
}

std::vector<sf::Vector2i> getNeighbors(int x, int y) {
    return {{x+1, y}, {x-1, y}, {x, y+1}, {x, y-1}};
}
