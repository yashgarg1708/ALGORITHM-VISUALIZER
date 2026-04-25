#include <iostream>
#include <sstream>
#include <string>

void runSortingVisualizer();
void runGraphVisualizer();

namespace {

int readMenuChoice(int minValue, int maxValue) {
    std::string line;

    while (true) {
        if (!std::getline(std::cin, line)) {
            return maxValue;
        }

        std::stringstream parser(line);
        int value = 0;
        char extra = '\0';

        if ((parser >> value) && !(parser >> extra) && value >= minValue && value <= maxValue) {
            return value;
        }

        std::cout << "Please enter a number between " << minValue << " and " << maxValue << ".\n";
        std::cout << "Choice: ";
    }
}

void showMainMenu() {
    std::cout << "\n==== Algorithm Visualizer ====\n";
    std::cout << "1. Sorting Algorithms\n";
    std::cout << "2. Graph Pathfinding Algorithms\n";
    std::cout << "3. Exit\n";
    std::cout << "Choice: ";
}

}  // namespace

int main() {
    while (true) {
        showMainMenu();
        const int choice = readMenuChoice(1, 3);

        if (choice == 1) {
            runSortingVisualizer();
        } else if (choice == 2) {
            runGraphVisualizer();
        } else {
            std::cout << "Exiting...\n";
            return 0;
        }
    }
}
