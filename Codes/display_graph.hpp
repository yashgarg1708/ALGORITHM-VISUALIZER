#ifndef DISPLAY_GRAPH_HPP
#define DISPLAY_GRAPH_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "graph.hpp"

// Function declarations for drawing the grid and handling visualization
void drawGrid(const std::vector<std::vector<CellType>>& grid, sf::RenderWindow &window);
void displayVisualization(int algo_choice);

#endif