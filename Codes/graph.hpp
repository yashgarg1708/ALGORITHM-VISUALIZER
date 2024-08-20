#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <SFML/Graphics.hpp>
#include <vector>

// Declare the enum in this file
enum CellType { EMPTY, START, END, BLOCK, PATH };

// Declare constants for grid dimensions and delay
extern const int GRID_WIDTH;
extern const int GRID_HEIGHT;
extern const int DELAY_MS;

// Function declarations
void runBFS(std::vector<std::vector<CellType>>& grid, sf::Vector2i startPos, sf::Vector2i endPos, sf::RenderWindow &window);
void runDFS(std::vector<std::vector<CellType>>& grid, sf::Vector2i startPos, sf::Vector2i endPos, sf::RenderWindow &window);
void runDijkstra(std::vector<std::vector<CellType>>& grid, sf::Vector2i startPos, sf::Vector2i endPos, sf::RenderWindow &window);

// Declare getNeighbors function
std::vector<sf::Vector2i> getNeighbors(int x, int y);

#endif