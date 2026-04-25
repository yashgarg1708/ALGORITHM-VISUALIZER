#include "sorting.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cstdint>
#include <optional>
#include <sstream>
#include <vector>

namespace {

constexpr int kWindowWidth = 1200;
constexpr int kWindowHeight = 700;
constexpr int kBarCount = 90;
constexpr float kBarWidth = 10.0f;
constexpr float kBarGap = 2.0f;
constexpr float kBottomMargin = 36.0f;
constexpr float kTopMargin = 20.0f;

struct SortUiState {
    std::vector<float> bars;
    std::vector<float> baseline;
    std::vector<SortOp> ops;

    std::size_t opIndex = 0;
    int highlightA = -1;
    int highlightB = -1;

    SortAlgorithm algorithm = SortAlgorithm::Bubble;

    bool running = false;
    bool paused = false;
    bool sorted = false;

    int opsPerSecond = 60;
    float opBudget = 0.0f;
};

void randomizeBars(SortUiState& state) {
    const float maxBarHeight = static_cast<float>(kWindowHeight) - kBottomMargin - kTopMargin;
    state.bars = generateRandomBars(kBarCount, 18.0f, maxBarHeight);
    state.baseline = state.bars;
    state.ops.clear();
    state.opIndex = 0;
    state.highlightA = -1;
    state.highlightB = -1;
    state.running = false;
    state.paused = false;
    state.sorted = false;
    state.opBudget = 0.0f;
}

void startSort(SortUiState& state) {
    state.bars = state.baseline;
    state.ops = generateSortOps(state.algorithm, state.bars);
    state.opIndex = 0;
    state.highlightA = -1;
    state.highlightB = -1;
    state.running = true;
    state.paused = false;
    state.sorted = false;
    state.opBudget = 0.0f;
}

sf::Color blendColor(const sf::Color& from, const sf::Color& to, float t) {
    t = std::max(0.0f, std::min(1.0f, t));
    return sf::Color(
        static_cast<std::uint8_t>(from.r + static_cast<int>((to.r - from.r) * t)),
        static_cast<std::uint8_t>(from.g + static_cast<int>((to.g - from.g) * t)),
        static_cast<std::uint8_t>(from.b + static_cast<int>((to.b - from.b) * t))
    );
}

void drawBackground(sf::RenderWindow& window) {
    sf::RectangleShape layerTop(sf::Vector2f(static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight * 0.65f)));
    layerTop.setFillColor(sf::Color(7, 14, 24));
    window.draw(layerTop);

    sf::RectangleShape layerBottom(sf::Vector2f(static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight * 0.35f)));
    layerBottom.setPosition({0.0f, static_cast<float>(kWindowHeight) * 0.65f});
    layerBottom.setFillColor(sf::Color(18, 33, 53));
    window.draw(layerBottom);

    sf::RectangleShape baseline(sf::Vector2f(static_cast<float>(kWindowWidth), 2.0f));
    baseline.setPosition({0.0f, static_cast<float>(kWindowHeight) - kBottomMargin});
    baseline.setFillColor(sf::Color(120, 144, 176));
    window.draw(baseline);
}

void drawBars(const SortUiState& state, sf::RenderWindow& window) {
    const float totalBarSpan = (kBarWidth + kBarGap) * static_cast<float>(state.bars.size());
    const float leftPadding = std::max(20.0f, (static_cast<float>(kWindowWidth) - totalBarSpan) * 0.5f);

    for (std::size_t i = 0; i < state.bars.size(); ++i) {
        sf::RectangleShape bar(sf::Vector2f(kBarWidth, state.bars[i]));
        const float x = leftPadding + static_cast<float>(i) * (kBarWidth + kBarGap);
        const float y = static_cast<float>(kWindowHeight) - kBottomMargin - state.bars[i];
        bar.setPosition({x, y});

        if (static_cast<int>(i) == state.highlightA || static_cast<int>(i) == state.highlightB) {
            bar.setFillColor(sf::Color(251, 146, 60));
        } else if (state.sorted) {
            const float t = static_cast<float>(i) / std::max(1.0f, static_cast<float>(state.bars.size() - 1));
            bar.setFillColor(blendColor(sf::Color(22, 163, 74), sf::Color(134, 239, 172), t));
        } else {
            const float t = state.bars[i] / std::max(1.0f, static_cast<float>(kWindowHeight));
            bar.setFillColor(blendColor(sf::Color(147, 197, 253), sf::Color(59, 130, 246), t));
        }

        window.draw(bar);
    }
}

void updateWindowTitle(sf::RenderWindow& window, const SortUiState& state) {
    std::ostringstream title;
    title << "Sorting Visualizer | Algo:" << sortAlgorithmName(state.algorithm)
          << " | Space:Start/Pause  Enter:Rerun  R:Randomize  Up/Down:Speed(" << state.opsPerSecond << "/s)"
          << " | 1..5:Algo  Esc:Back";

    if (state.running && state.paused) {
        title << " | Paused";
    } else if (state.sorted) {
        title << " | Sorted";
    }

    window.setTitle(title.str());
}

void advanceSort(SortUiState& state, float deltaSeconds) {
    if (!state.running || state.paused) {
        return;
    }

    state.opBudget += std::max(0.0f, deltaSeconds) * static_cast<float>(state.opsPerSecond);
    int steps = std::min(200, static_cast<int>(state.opBudget));
    state.opBudget -= static_cast<float>(steps);

    while (steps-- > 0 && state.opIndex < state.ops.size()) {
        const SortOp& op = state.ops[state.opIndex++];
        applySortOp(state.bars, op);
        state.highlightA = op.i;
        state.highlightB = op.j;
    }

    if (state.opIndex >= state.ops.size()) {
        state.running = false;
        state.paused = false;
        state.sorted = isSortedAscending(state.bars);
        state.highlightA = -1;
        state.highlightB = -1;
        state.opBudget = 0.0f;
    }
}

SortAlgorithm keyToSortAlgorithm(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Key::Num1:
            return SortAlgorithm::Insertion;
        case sf::Keyboard::Key::Num2:
            return SortAlgorithm::Bubble;
        case sf::Keyboard::Key::Num3:
            return SortAlgorithm::Selection;
        case sf::Keyboard::Key::Num4:
            return SortAlgorithm::Heap;
        default:
            return SortAlgorithm::Quick;
    }
}

int upSpeedStep(int speed) {
    if (speed < 10) {
        return 1;
    }
    if (speed < 100) {
        return 5;
    }
    return 25;
}

int downSpeedStep(int speed) {
    if (speed <= 10) {
        return 1;
    }
    if (speed <= 100) {
        return 5;
    }
    return 25;
}

}  // namespace

void runSortingVisualizer() {
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(kWindowWidth), static_cast<unsigned int>(kWindowHeight)}),
        "Sorting Visualizer"
    );
    window.setFramerateLimit(60);

    SortUiState state;
    sf::Clock frameClock;
    randomizeBars(state);

    while (window.isOpen()) {
        const float deltaSeconds = frameClock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                } else if (keyPressed->code >= sf::Keyboard::Key::Num1 && keyPressed->code <= sf::Keyboard::Key::Num5) {
                    state.algorithm = keyToSortAlgorithm(keyPressed->code);
                    state.running = false;
                    state.paused = false;
                    state.sorted = false;
                } else if (keyPressed->code == sf::Keyboard::Key::Space) {
                    if (state.running) {
                        state.paused = !state.paused;
                    } else {
                        startSort(state);
                    }
                } else if (keyPressed->code == sf::Keyboard::Key::Enter) {
                    startSort(state);
                } else if (keyPressed->code == sf::Keyboard::Key::R) {
                    randomizeBars(state);
                } else if (keyPressed->code == sf::Keyboard::Key::Up) {
                    state.opsPerSecond = std::min(600, state.opsPerSecond + upSpeedStep(state.opsPerSecond));
                } else if (keyPressed->code == sf::Keyboard::Key::Down) {
                    state.opsPerSecond = std::max(1, state.opsPerSecond - downSpeedStep(state.opsPerSecond));
                }
            }
        }

        advanceSort(state, deltaSeconds);
        updateWindowTitle(window, state);

        window.clear();
        drawBackground(window);
        drawBars(state, window);
        window.display();
    }
}
