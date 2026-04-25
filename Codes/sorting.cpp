#include "sorting.hpp"

#include <algorithm>
#include <random>

namespace {

void addSwap(std::vector<SortOp>& ops, int i, int j) {
    if (i == j) {
        return;
    }

    SortOp op;
    op.type = SortOp::Type::Swap;
    op.i = i;
    op.j = j;
    ops.push_back(op);
}

void addSet(std::vector<SortOp>& ops, int i, float value) {
    SortOp op;
    op.type = SortOp::Type::Set;
    op.i = i;
    op.value = value;
    ops.push_back(op);
}

std::vector<SortOp> insertionSortOps(const std::vector<float>& input) {
    std::vector<float> arr = input;
    std::vector<SortOp> ops;

    for (std::size_t i = 1; i < arr.size(); ++i) {
        float key = arr[i];
        int j = static_cast<int>(i) - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            addSet(ops, j + 1, arr[j]);
            --j;
        }

        arr[j + 1] = key;
        addSet(ops, j + 1, key);
    }

    return ops;
}

std::vector<SortOp> bubbleSortOps(const std::vector<float>& input) {
    std::vector<float> arr = input;
    std::vector<SortOp> ops;

    if (arr.empty()) {
        return ops;
    }

    for (std::size_t i = 0; i < arr.size() - 1; ++i) {
        bool swapped = false;
        for (std::size_t j = 0; j < arr.size() - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                addSwap(ops, static_cast<int>(j), static_cast<int>(j + 1));
                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }

    return ops;
}

std::vector<SortOp> selectionSortOps(const std::vector<float>& input) {
    std::vector<float> arr = input;
    std::vector<SortOp> ops;

    if (arr.empty()) {
        return ops;
    }

    for (std::size_t i = 0; i < arr.size() - 1; ++i) {
        std::size_t minIndex = i;
        for (std::size_t j = i + 1; j < arr.size(); ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
            addSwap(ops, static_cast<int>(i), static_cast<int>(minIndex));
        }
    }

    return ops;
}

void heapify(std::vector<float>& arr, int n, int i, std::vector<SortOp>& ops) {
    int largest = i;
    int left = (2 * i) + 1;
    int right = (2 * i) + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        addSwap(ops, i, largest);
        heapify(arr, n, largest, ops);
    }
}

std::vector<SortOp> heapSortOps(const std::vector<float>& input) {
    std::vector<float> arr = input;
    std::vector<SortOp> ops;

    int n = static_cast<int>(arr.size());
    for (int i = (n / 2) - 1; i >= 0; --i) {
        heapify(arr, n, i, ops);
    }

    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        addSwap(ops, 0, i);
        heapify(arr, i, 0, ops);
    }

    return ops;
}

int partition(std::vector<float>& arr, int low, int high, std::vector<SortOp>& ops) {
    float pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            if (i != j) {
                std::swap(arr[i], arr[j]);
                addSwap(ops, i, j);
            }
        }
    }

    if ((i + 1) != high) {
        std::swap(arr[i + 1], arr[high]);
        addSwap(ops, i + 1, high);
    }

    return i + 1;
}

void quickSort(std::vector<float>& arr, int low, int high, std::vector<SortOp>& ops) {
    if (low >= high) {
        return;
    }

    int pi = partition(arr, low, high, ops);
    quickSort(arr, low, pi - 1, ops);
    quickSort(arr, pi + 1, high, ops);
}

std::vector<SortOp> quickSortOps(const std::vector<float>& input) {
    std::vector<float> arr = input;
    std::vector<SortOp> ops;

    if (!arr.empty()) {
        quickSort(arr, 0, static_cast<int>(arr.size()) - 1, ops);
    }

    return ops;
}

}  // namespace

const char* sortAlgorithmName(SortAlgorithm algorithm) {
    switch (algorithm) {
        case SortAlgorithm::Insertion:
            return "Insertion";
        case SortAlgorithm::Bubble:
            return "Bubble";
        case SortAlgorithm::Selection:
            return "Selection";
        case SortAlgorithm::Heap:
            return "Heap";
        case SortAlgorithm::Quick:
            return "Quick";
        default:
            return "Unknown";
    }
}

std::vector<SortOp> generateSortOps(SortAlgorithm algorithm, const std::vector<float>& values) {
    switch (algorithm) {
        case SortAlgorithm::Insertion:
            return insertionSortOps(values);
        case SortAlgorithm::Bubble:
            return bubbleSortOps(values);
        case SortAlgorithm::Selection:
            return selectionSortOps(values);
        case SortAlgorithm::Heap:
            return heapSortOps(values);
        case SortAlgorithm::Quick:
            return quickSortOps(values);
        default:
            return {};
    }
}

void applySortOp(std::vector<float>& values, const SortOp& op) {
    if (op.i < 0 || op.i >= static_cast<int>(values.size())) {
        return;
    }

    if (op.type == SortOp::Type::Swap) {
        if (op.j < 0 || op.j >= static_cast<int>(values.size())) {
            return;
        }

        std::swap(values[op.i], values[op.j]);
        return;
    }

    values[op.i] = op.value;
}

bool isSortedAscending(const std::vector<float>& values) {
    for (std::size_t i = 1; i < values.size(); ++i) {
        if (values[i - 1] > values[i]) {
            return false;
        }
    }

    return true;
}

std::vector<float> generateRandomBars(std::size_t count, float minValue, float maxValue) {
    if (minValue > maxValue) {
        std::swap(minValue, maxValue);
    }

    std::vector<float> bars(count);
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(minValue, maxValue);

    for (float& value : bars) {
        value = dist(rng);
    }

    return bars;
}
