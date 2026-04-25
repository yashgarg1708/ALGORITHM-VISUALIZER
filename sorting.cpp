#include <SFML/Graphics.hpp>
#include <algorithm>

extern float recHs[];
extern bool sorted;
extern void dispSort(sf::RenderWindow& window, float recHs[], int n, int index1, int index2, bool sorted, int updateFrequency);

void insertionSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted) {
    int i, j;
    float key;
    for (i = 1; i < n; i++) {
        key = recHs[i];
        j = i - 1;
        while (j >= 0 && recHs[j] > key) {
            recHs[j+1] = recHs[j];
            j--;
            dispSort(window, recHs, n, j+1, i, false, 2);
        }
        recHs[j+1] = key;
    }
    sorted = true;
    dispSort(window, recHs, n, -1, -1, true, 2);
}

void bubbleSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (recHs[j] > recHs[j+1]) {
                std::swap(recHs[j], recHs[j+1]);
                dispSort(window, recHs, n, j, j+1, false, 2);
            }
        }
    }
    sorted = true;
    dispSort(window, recHs, n, -1, -1, true, 2);
}

void selectionSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted) {
    for (int i = 0; i < n-1; i++) {
        int min_idx = i;
        for (int j = i+1; j < n; j++)
            if (recHs[j] < recHs[min_idx]) min_idx = j;
        std::swap(recHs[min_idx], recHs[i]);
        dispSort(window, recHs, n, min_idx, i, false, 2);
    }
    sorted = true;
    dispSort(window, recHs, n, -1, -1, true, 2);
}

void heapify(sf::RenderWindow& window, float arr[], int n, int i, bool& sorted) {
    int largest = i, l = 2*i+1, r = 2*i+2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        dispSort(window, arr, n, i, largest, false, 2);
        heapify(window, arr, n, largest, sorted);
    }
}

void heapSort(sf::RenderWindow& window, float recHs[], int n, bool& sorted) {
    for (int i = n/2-1; i >= 0; i--) heapify(window, recHs, n, i, sorted);
    for (int i = n-1; i > 0; i--) {
        std::swap(recHs[0], recHs[i]);
        dispSort(window, recHs, n, 0, i, false, 2);
        heapify(window, recHs, i, 0, sorted);
    }
    sorted = true;
    dispSort(window, recHs, n, -1, -1, true, 2);
}

int partition(sf::RenderWindow& window, float arr[], int low, int high, int n, bool& sorted) {
    float pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            std::swap(arr[++i], arr[j]);
            dispSort(window, arr, n, i, j, false, 2);
        }
    }
    std::swap(arr[i+1], arr[high]);
    dispSort(window, arr, n, i+1, high, false, 2);
    return i + 1;
}

void quickSort(sf::RenderWindow& window, float recHs[], int low, int high, int n, bool& sorted) {
    if (low < high) {
        int pi = partition(window, recHs, low, high, n, sorted);
        quickSort(window, recHs, low, pi-1, n, sorted);
        quickSort(window, recHs, pi+1, high, n, sorted);
    }
    if (low == 0 && high == n-1) {
        sorted = true;
        dispSort(window, recHs, n, -1, -1, true, 2);
    }
}
