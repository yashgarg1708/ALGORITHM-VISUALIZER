#include "display_graph.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

// Constants for grid dimensions and delay
const int GRID_WIDTH = 40;
const int GRID_HEIGHT = 30;
const int CELL_SIZE = 20;
const int DELAY_MS = 50;

// Function to draw the grid on the window
void drawGrid(const std::vector<std::vector<CellType>>& grid, sf::RenderWindow &window) {
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(i * CELL_SIZE, j * CELL_SIZE);
            cell.setOutlineThickness(1);  // Set outline thickness for visibility
            cell.setOutlineColor(sf::Color::Black);  // Set outline color to black
            
            if (grid[i][j] == START) {
                cell.setFillColor(sf::Color::Green);
            } else if (grid[i][j] == END) {
                cell.setFillColor(sf::Color::Red);  // Final path in yellow
            } else if (grid[i][j] == BLOCK) {
                cell.setFillColor(sf::Color::Black);
            } else if (grid[i][j] == PATH) {
                cell.setFillColor(sf::Color::Blue);
            } else {
                cell.setFillColor(sf::Color::White);
            }

            window.draw(cell);
        }
    }
}

// Function to handle visualization of the algorithms
void displayVisualization(int algo_choice) {
    sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE), "Algorithm Visualization");

    std::vector<std::vector<CellType>> grid(GRID_WIDTH, std::vector<CellType>(GRID_HEIGHT, EMPTY));

    bool startPlaced = false, endPlaced = false;
    sf::Vector2i startPos, endPos;

    bool algorithmStarted = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle mouse clicks for placing start, end, and blocks
            if (!algorithmStarted && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int x = mousePos.x / CELL_SIZE;
                int y = mousePos.y / CELL_SIZE;

                if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                    if (!startPlaced) {
                        grid[x][y] = START;
                        startPos = sf::Vector2i(x, y);
                        startPlaced = true;
                    } else if (!endPlaced && grid[x][y] == EMPTY) {
                        grid[x][y] = END;
                        endPos = sf::Vector2i(x, y);
                        endPlaced = true;
                    } else if (grid[x][y] == EMPTY) {
                        grid[x][y] = BLOCK;
                    }
                }
            }

            // Trigger the algorithm with the spacebar
            if (!algorithmStarted && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (startPlaced && endPlaced) {
                    algorithmStarted = true;

                    if (algo_choice == 1) {
                        runBFS(grid, startPos, endPos, window);
                    } else if (algo_choice == 2) {
                        runDFS(grid, startPos, endPos, window);
                    } else if (algo_choice == 3) {
                        runDijkstra(grid, startPos, endPos, window);
                    }
                }
            }
        }

        window.clear();
        drawGrid(grid, window);
        window.display();
    }
}