#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

void dispSort(sf::RenderWindow &window, float recHs[], int n, int index1, int index2, bool sorted, int updateFrequency = 1) {
    static int updateCount = 0;
    updateCount++;

    if (updateCount % updateFrequency == 0 || sorted) {
        window.clear(sf::Color(0, 128, 128));  // Set background color to teal

        for (int i = 0; i < n; i++) {
            sf::RectangleShape block(sf::Vector2f(10, recHs[i]));
            block.setPosition(i * 12, 600 - recHs[i]);

            if (i == index1 || i == index2) {
                block.setFillColor(sf::Color::Blue);  // Set selected element color to blue
            } else {
                block.setFillColor(sorted ? sf::Color::Green : sf::Color::White);
            }

            window.draw(block);
        }
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // Adjust delay as needed
        updateCount = 0;  // Reset counter
    }
}