#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <SFML/Graphics.hpp>
#include <vector>

enum CellType { EMPTY, START, END, BLOCK, PATH };

extern const int GRID_WIDTH;
extern const int GRID_HEIGHT;
extern const int DELAY_MS;

void runSearch(std::vector<std::vector<CellType>>& grid, sf::Vector2i start, sf::Vector2i end, sf::RenderWindow& window, bool bfs);
void runDijkstra(std::vector<std::vector<CellType>>& grid, sf::Vector2i startPos, sf::Vector2i endPos, sf::RenderWindow& window);
std::vector<sf::Vector2i> getNeighbors(int x, int y);

#endif
