#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "graph.hpp"
#include "display_graph.hpp"
using namespace std;

const int n = 14;
float recHs[n];
bool sorted = false;

void insertionSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted);
void bubbleSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted);
void selectionSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted);
void heapSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted);
void quickSort(sf::RenderWindow& window, float recHs[], int low, int high, int n, bool& sorted);
void dispSort(sf::RenderWindow& window, float recHs[], int n, int index1, int index2, bool sorted);

void resetArray() {
    for (int i = 0; i < n; i++)
        recHs[i] = static_cast<float>(80 + rand() % 440);
    sorted = false;
}

void runGraph() {
    int choice;
    while (true) {
        cout << "Select an algorithm to visualize:\n";
        cout << "1. Breadth-First Search (BFS)\n";
        cout << "2. Depth-First Search (DFS)\n";
        cout << "3. Dijkstra's Algorithm\n";
        cout << "4. Quit to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 4) return;
        if (choice >= 1 && choice <= 3)
            displayVisualization(choice);
        else
            cout << "Invalid choice. Please try again.\n";
    }
}

void runSort() {
    sf::RenderWindow window(sf::VideoMode({960, 600}), "Sorter");
    int choice;
    while (true) {
        cout << "Menu:\n";
        cout << "1. Insertion Sort\n";
        cout << "2. Bubble Sort\n";
        cout << "3. Selection Sort\n";
        cout << "4. Heap Sort\n";
        cout << "5. Quick Sort\n";
        cout << "6. Quit to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 6) { window.close(); break; }
        if (choice < 1 || choice > 5) { cout << "Invalid choice! Please choose a valid sorting algorithm.\n"; continue; }
        resetArray();
        while (const auto event = window.pollEvent())
            if (event->is<sf::Event::Closed>()) return;
        switch (choice) {
            case 1: insertionSort(window, recHs, n, sorted); break;
            case 2: bubbleSort(window, recHs, n, sorted); break;
            case 3: selectionSort(window, recHs, n, sorted); break;
            case 4: heapSort(window, recHs, n, sorted); break;
            case 5: quickSort(window, recHs, 0, n-1, n, sorted); break;
        }
    }
}

int main() {
    int answer;
    while (true) {
        cout << "Enter 1 for Sorting Algorithms\n";
        cout << "Enter 2 for Graph Searching Algorithms\n";
        cout << "Enter 3 to Exit\n";
        cin >> answer;
        if (answer == 1) runSort();
        else if (answer == 2) runGraph();
        else if (answer == 3) { cout << "Exiting ...\n"; break; }
        else cout << "Invalid choice! Please try again.\n";
    }
    return 0;
}
