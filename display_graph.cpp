#include "display_graph.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

const int GRID_WIDTH = 40;
const int GRID_HEIGHT = 30;
const int CELL_SIZE = 20;
const int DELAY_MS = 50;

void drawGrid(const std::vector<std::vector<CellType>>& grid, sf::RenderWindow& window) {
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition({static_cast<float>(i * CELL_SIZE), static_cast<float>(j * CELL_SIZE)});
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::Black);
            sf::Color color = sf::Color::White;
            if      (grid[i][j] == START) color = sf::Color::Green;
            else if (grid[i][j] == END)   color = sf::Color::Red;
            else if (grid[i][j] == BLOCK) color = sf::Color::Black;
            else if (grid[i][j] == PATH)  color = sf::Color::Blue;
            cell.setFillColor(color);
            window.draw(cell);
        }
    }
}

void displayVisualization(int algo_choice) {
    sf::RenderWindow window(sf::VideoMode({GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE}), "Algorithm Visualization");
    std::vector<std::vector<CellType>> grid(GRID_WIDTH, std::vector<CellType>(GRID_HEIGHT, EMPTY));
    bool startPlaced = false, endPlaced = false, algorithmStarted = false;
    sf::Vector2i startPos, endPos;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (!algorithmStarted) {
                const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouse && mouse->button == sf::Mouse::Button::Left) {
                    int x = mouse->position.x / CELL_SIZE, y = mouse->position.y / CELL_SIZE;
                    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                        if (!startPlaced)                          { grid[x][y] = START; startPos = {x, y}; startPlaced = true; }
                        else if (!endPlaced && grid[x][y] == EMPTY) { grid[x][y] = END;   endPos   = {x, y}; endPlaced   = true; }
                        else if (grid[x][y] == EMPTY)              grid[x][y] = BLOCK;
                    }
                }
            }

            const auto* key = event->getIf<sf::Event::KeyPressed>();
            if (!algorithmStarted && key && key->code == sf::Keyboard::Key::Space && startPlaced && endPlaced) {
                algorithmStarted = true;
                if (algo_choice <= 2) runSearch(grid, startPos, endPos, window, algo_choice == 1);
                else                  runDijkstra(grid, startPos, endPos, window);
            }
        }
        window.clear();
        drawGrid(grid, window);
        window.display();
    }
}
