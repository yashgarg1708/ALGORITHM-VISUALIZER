#include <iostream>
#include <SFML/Graphics.hpp>
#include "graph.hpp"
#include <cstdlib>  // For rand()
#include "display_graph.hpp"

const int n = 80; // Number of elements to sort
float recHs[n];
bool sorted = false;

// Declare all sorting functions
void insertionSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted);
void bubbleSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted);
void selectionSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted);
void heapSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted);
void quickSort(sf::RenderWindow& window, float recHs[], int low, int high, int n, bool& sorted);

// Declare the display function
void dispSort(sf::RenderWindow &window, float recHs[], int n, int index1, int index2, bool sorted, int updateFrequency = 1);

void resetArray() {
    for (int i = 0; i < n; i++) {
        recHs[i] = static_cast<float>(rand() % 500);
    }
    sorted = false;
}

void runGraph() {
    int choice;

    while (true) {  // Keep the graph menu active until the user quits
        std::cout << "Select an algorithm to visualize:" << std::endl;
        std::cout << "1. Breadth-First Search (BFS)" << std::endl;
        std::cout << "2. Depth-First Search (DFS)" << std::endl;
        std::cout << "3. Dijkstra's Algorithm" << std::endl;
        std::cout << "4. Quit to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                displayVisualization(1);  // BFS
                break;
            case 2:
                displayVisualization(2);  // DFS
                break;
            case 3:
                displayVisualization(3);  // Dijkstra
                break;
            case 4:
                std::cout << "Returning to main menu..." << std::endl;
                return;  // Return to main menu
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

void runSort() {
    sf::RenderWindow window(sf::VideoMode(960, 600), "Sorter");

    int choice;

    while (true) {  // Keep the sort menu active until the user quits
        std::cout << "Menu:\n";
        std::cout << "1. Insertion Sort\n";
        std::cout << "2. Bubble Sort\n";
        std::cout << "3. Selection Sort\n";
        std::cout << "4. Heap Sort\n";
        std::cout << "5. Quick Sort\n";
        std::cout << "6. Quit to Main Menu\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 6) {
            window.close();  // Close the SFML window and break out of the sorting menu
            break;
        }

        resetArray();  // Reset array before each sort

        // Keep sorting in the window
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            if (!sorted) {
                switch (choice) {
                    case 1:
                        insertionSort(window, recHs, n, sorted);
                        break;
                    case 2:
                        bubbleSort(window, recHs, n, sorted);
                        break;
                    case 3:
                        selectionSort(window, recHs, n, sorted);
                        break;
                    case 4:
                        heapSort(window, recHs, n, sorted);
                        break;
                    case 5:
                        quickSort(window, recHs, 0, n - 1, n, sorted);
                        break;
                    default:
                        std::cout << "Invalid choice! Please choose a valid sorting algorithm.\n";
                        window.close();
                        break;
                }
                // Call dispSort with the updateFrequency parameter
                dispSort(window, recHs, n, -1, -1, true, 10);  // Example update frequency
            }

            if (sorted) {
                break;  // Exit the loop after sorting is complete
            }
        }
    }
}

int main() {
    int answer;
    while (true) {
        std::cout << "Enter 1 for Sorting Algorithms" << std::endl;
        std::cout << "Enter 2 for Graph Searching Algorithms" << std::endl;
        std::cout << "Enter 3 to Exit" << std::endl;
        std::cin >> answer;

        if (answer == 1) {
            runSort();  // Call sorting algorithms menu
        }
        else if (answer == 2) {
            runGraph();  // Call graph searching algorithms menu
        }
        else if (answer == 3) {
            std::cout << "Exiting ..." << std::endl;
            break;  // Exit the main loop and terminate the program
        }
        else {
            std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }

    return 0;  // Program successfully exits
}

//g++ -std=c++11 main.cpp graph.cpp display_graph.cpp sorting.cpp display.cpp -I/opt/homebrew/Cellar/sfml/2.6.1/include -o prog -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system


