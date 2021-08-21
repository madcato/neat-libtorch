#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>

#include "neat/genotype/genome.hpp"

float rand_uni_val();
bool rand_bool();
int rand(int min, int max);
float rand(float min, float max);

template <typename T>
T choice(const std::vector<T>& array) {
    int pos = rand(0, array.size() - 1);
    return array[pos];
}

Genome get_best_genome(const std::vector<Genome>& population);

#endif  // UTILS_HPP