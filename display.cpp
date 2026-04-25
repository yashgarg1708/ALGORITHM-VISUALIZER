#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

void dispSort(sf::RenderWindow& window, float recHs[], int n, int index1, int index2, bool sorted, int updateFrequency) {
    static int updateCount = 0;
    if (++updateCount % updateFrequency == 0 || sorted) {
        window.clear(sf::Color(0, 128, 128));
        for (int i = 0; i < n; i++) {
            sf::RectangleShape block(sf::Vector2f(10, recHs[i]));
            block.setPosition({static_cast<float>(i * 12), 600 - recHs[i]});
            block.setFillColor(i == index1 || i == index2 ? sf::Color::Blue
                               : (sorted ? sf::Color::Green : sf::Color::White));
            window.draw(block);
        }
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        updateCount = 0;
    }
}
