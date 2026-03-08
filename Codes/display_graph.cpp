#include "graph.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <optional>
#include <random>
#include <sstream>

namespace {

constexpr int kGridWidth = 40;
constexpr int kGridHeight = 30;
constexpr int kCellSize = 22;
constexpr int kWindowWidth = kGridWidth * kCellSize;
constexpr int kWindowHeight = kGridHeight * kCellSize;

struct GraphUiState {
    Grid grid;
    bool startPlaced = false;
    bool endPlaced = false;
    GridPoint start{0, 0};
    GridPoint end{0, 0};

    GraphAlgorithm algorithm = GraphAlgorithm::BFS;

    bool running = false;
    bool paused = false;

    GraphTraversalResult result;
    std::size_t visitIndex = 0;
    std::size_t pathIndex = 0;
    int stepsPerFrame = 3;
};

Grid makeEmptyGrid() {
    return Grid(kGridWidth, std::vector<CellType>(kGridHeight, CellType::Empty));
}

bool isStartOrEnd(const GraphUiState& state, const GridPoint& p) {
    return (state.startPlaced && p == state.start) || (state.endPlaced && p == state.end);
}

void clearTraversalMarks(GraphUiState& state) {
    for (int x = 0; x < kGridWidth; ++x) {
        for (int y = 0; y < kGridHeight; ++y) {
            if (state.grid[x][y] == CellType::Visited || state.grid[x][y] == CellType::Path) {
                state.grid[x][y] = CellType::Empty;
            }
        }
    }

    if (state.startPlaced) {
        state.grid[state.start.x][state.start.y] = CellType::Start;
    }
    if (state.endPlaced) {
        state.grid[state.end.x][state.end.y] = CellType::End;
    }
}

void clearAll(GraphUiState& state) {
    state.grid = makeEmptyGrid();
    state.startPlaced = false;
    state.endPlaced = false;
    state.running = false;
    state.paused = false;
    state.result = GraphTraversalResult{};
    state.visitIndex = 0;
    state.pathIndex = 0;
}

void randomizeBlocks(GraphUiState& state, float density = 0.25f) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    clearTraversalMarks(state);

    for (int x = 0; x < kGridWidth; ++x) {
        for (int y = 0; y < kGridHeight; ++y) {
            GridPoint p{x, y};
            if (isStartOrEnd(state, p)) {
                continue;
            }
            state.grid[x][y] = (dist(rng) < density) ? CellType::Block : CellType::Empty;
        }
    }
}

void startTraversal(GraphUiState& state) {
    if (!state.startPlaced || !state.endPlaced) {
        return;
    }

    clearTraversalMarks(state);

    state.result = traverseGrid(state.grid, state.start, state.end, state.algorithm);
    state.visitIndex = 0;
    state.pathIndex = 0;
    state.running = true;
    state.paused = false;
}

sf::Color colorForCell(CellType cell) {
    switch (cell) {
        case CellType::Empty:
            return sf::Color(237, 242, 247);
        case CellType::Start:
            return sf::Color(22, 163, 74);
        case CellType::End:
            return sf::Color(220, 38, 38);
        case CellType::Block:
            return sf::Color(31, 41, 55);
        case CellType::Visited:
            return sf::Color(56, 189, 248);
        case CellType::Path:
            return sf::Color(234, 179, 8);
        default:
            return sf::Color::White;
    }
}

void drawGrid(const GraphUiState& state, sf::RenderWindow& window) {
    sf::RectangleShape backdrop(sf::Vector2f(static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight)));
    backdrop.setFillColor(sf::Color(15, 23, 42));
    window.draw(backdrop);

    for (int x = 0; x < kGridWidth; ++x) {
        for (int y = 0; y < kGridHeight; ++y) {
            sf::RectangleShape cell(sf::Vector2f(static_cast<float>(kCellSize - 1), static_cast<float>(kCellSize - 1)));
            cell.setPosition({static_cast<float>(x * kCellSize), static_cast<float>(y * kCellSize)});
            cell.setFillColor(colorForCell(state.grid[x][y]));
            window.draw(cell);
        }
    }
}

void updateWindowTitle(sf::RenderWindow& window, const GraphUiState& state) {
    std::ostringstream title;
    title << "Graph Visualizer | Algo: " << graphAlgorithmName(state.algorithm)
          << " | Space:Start/Pause  Enter:Rerun  R:Random blocks  C:Clear  Up/Down:Speed(" << state.stepsPerFrame << ")"
          << " | 1/2/3:Algo  Esc:Back";

    if (!state.startPlaced || !state.endPlaced) {
        title << " | Left click: set Start/End/Blocks, Right click: erase";
    } else if (!state.running && !state.result.found && !state.result.visitOrder.empty()) {
        title << " | No path found";
    }

    window.setTitle(title.str());
}

GridPoint mouseToGrid(int mouseX, int mouseY) {
    return GridPoint{mouseX / kCellSize, mouseY / kCellSize};
}

void onLeftClick(GraphUiState& state, const GridPoint& p) {
    if (!isInsideGrid(state.grid, p) || state.running) {
        return;
    }

    if (!state.startPlaced) {
        state.startPlaced = true;
        state.start = p;
        state.grid[p.x][p.y] = CellType::Start;
        return;
    }

    if (!state.endPlaced && p != state.start) {
        state.endPlaced = true;
        state.end = p;
        state.grid[p.x][p.y] = CellType::End;
        return;
    }

    if (p == state.start || p == state.end) {
        return;
    }

    state.grid[p.x][p.y] = (state.grid[p.x][p.y] == CellType::Block) ? CellType::Empty : CellType::Block;
}

void onRightClick(GraphUiState& state, const GridPoint& p) {
    if (!isInsideGrid(state.grid, p) || state.running) {
        return;
    }

    if (state.startPlaced && p == state.start) {
        state.startPlaced = false;
    }
    if (state.endPlaced && p == state.end) {
        state.endPlaced = false;
    }

    state.grid[p.x][p.y] = CellType::Empty;
}

void advanceTraversal(GraphUiState& state) {
    if (!state.running || state.paused) {
        return;
    }

    int steps = std::max(1, state.stepsPerFrame);

    while (steps > 0 && state.visitIndex < state.result.visitOrder.size()) {
        const GridPoint p = state.result.visitOrder[state.visitIndex++];
        if (!isStartOrEnd(state, p) && state.grid[p.x][p.y] != CellType::Block) {
            state.grid[p.x][p.y] = CellType::Visited;
        }
        --steps;
    }

    if (state.visitIndex < state.result.visitOrder.size()) {
        return;
    }

    if (state.result.found) {
        int pathSteps = std::max(1, state.stepsPerFrame / 2);
        while (pathSteps > 0 && state.pathIndex < state.result.path.size()) {
            const GridPoint p = state.result.path[state.pathIndex++];
            if (!isStartOrEnd(state, p) && state.grid[p.x][p.y] != CellType::Block) {
                state.grid[p.x][p.y] = CellType::Path;
            }
            --pathSteps;
        }

        if (state.pathIndex < state.result.path.size()) {
            return;
        }
    }

    state.running = false;
}

}  // namespace

void runGraphVisualizer() {
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(kWindowWidth), static_cast<unsigned int>(kWindowHeight)}),
        "Graph Visualizer"
    );
    window.setFramerateLimit(60);

    GraphUiState state;
    state.grid = makeEmptyGrid();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                } else if (keyPressed->code == sf::Keyboard::Key::Num1) {
                    state.algorithm = GraphAlgorithm::BFS;
                } else if (keyPressed->code == sf::Keyboard::Key::Num2) {
                    state.algorithm = GraphAlgorithm::DFS;
                } else if (keyPressed->code == sf::Keyboard::Key::Num3) {
                    state.algorithm = GraphAlgorithm::Dijkstra;
                } else if (keyPressed->code == sf::Keyboard::Key::Space) {
                    if (state.running) {
                        state.paused = !state.paused;
                    } else {
                        startTraversal(state);
                    }
                } else if (keyPressed->code == sf::Keyboard::Key::Enter) {
                    startTraversal(state);
                } else if (keyPressed->code == sf::Keyboard::Key::R) {
                    randomizeBlocks(state);
                    state.running = false;
                    state.paused = false;
                    state.result = GraphTraversalResult{};
                    state.visitIndex = 0;
                    state.pathIndex = 0;
                } else if (keyPressed->code == sf::Keyboard::Key::C) {
                    clearAll(state);
                } else if (keyPressed->code == sf::Keyboard::Key::Up) {
                    state.stepsPerFrame = std::min(20, state.stepsPerFrame + 1);
                } else if (keyPressed->code == sf::Keyboard::Key::Down) {
                    state.stepsPerFrame = std::max(1, state.stepsPerFrame - 1);
                }
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                GridPoint p = mouseToGrid(mousePressed->position.x, mousePressed->position.y);

                if (mousePressed->button == sf::Mouse::Button::Left) {
                    onLeftClick(state, p);
                } else if (mousePressed->button == sf::Mouse::Button::Right) {
                    onRightClick(state, p);
                }
            }
        }

        advanceTraversal(state);
        updateWindowTitle(window, state);

        window.clear();
        drawGrid(state, window);
        window.display();
    }
}
