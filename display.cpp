#include <SFML/Graphics.hpp>
#include <algorithm>
#include <chrono>
#include <thread>

void dispSort(sf::RenderWindow& window, float recHs[], int n, int index1, int index2, bool sorted) {
    static int delayMs = 600;
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window.close();
        const auto* key = event->getIf<sf::Event::KeyPressed>();
        if (key && key->code == sf::Keyboard::Key::Up && delayMs > 0) delayMs -= 50;
        else if (key && key->code == sf::Keyboard::Key::Down && delayMs < 2000) delayMs += 50;
    }
    const sf::Vector2u size = window.getSize();
    const float width = static_cast<float>(size.x);
    const float height = static_cast<float>(size.y);
    const float chartTop = 64.f;
    const float chartBottom = height - 54.f;
    const float chartHeight = chartBottom - chartTop;
    const float sidePadding = 70.f;
    const float slotWidth = (width - sidePadding * 2.f) / static_cast<float>(n);
    const float barWidth = std::max(16.f, slotWidth * 0.55f);

    window.clear(sf::Color(6, 14, 22));

    for (int i = 0; i < n; i++) {
        const float barHeight = std::min(recHs[i], chartHeight);
        const float x = sidePadding + slotWidth * static_cast<float>(i) + (slotWidth - barWidth) / 2.f;
        const float y = chartBottom - barHeight;

        sf::RectangleShape block(sf::Vector2f(barWidth, barHeight));
        block.setPosition({x, y});
        block.setFillColor(i == index1 || i == index2 ? sf::Color(255, 142, 32)
                           : (sorted ? sf::Color(33, 226, 139) : sf::Color(45, 151, 255)));
        window.draw(block);
    }
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
}
