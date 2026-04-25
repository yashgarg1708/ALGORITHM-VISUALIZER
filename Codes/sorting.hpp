#ifndef SORTING_HPP
#define SORTING_HPP

#include <cstddef>
#include <vector>

enum class SortAlgorithm {
    Insertion = 1,
    Bubble = 2,
    Selection = 3,
    Heap = 4,
    Quick = 5
};

struct SortOp {
    enum class Type {
        Swap,
        Set
    };

    Type type = Type::Swap;
    int i = -1;
    int j = -1;
    float value = 0.0f;
};

const char* sortAlgorithmName(SortAlgorithm algorithm);
std::vector<SortOp> generateSortOps(SortAlgorithm algorithm, const std::vector<float>& values);
void applySortOp(std::vector<float>& values, const SortOp& op);
bool isSortedAscending(const std::vector<float>& values);
std::vector<float> generateRandomBars(std::size_t count, float minValue, float maxValue);

#endif
