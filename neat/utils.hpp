#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>

float rand_uni_val();
bool rand_bool();
int rand(int min, int max);
float rand(float min, float max);

template <typename T>
T choice(const std::vector<T>& array) {
    int pos = rand(0, array.size() - 1);
    return array[pos];
}


// TODO: implement it
// def get_best_genome(population):
//     """
//     Gets best genome out of a population
//     :param population: List of Genome instances
//     :return: Genome instance
//     """
//     population_copy = copy.deepcopy(population)
//     population_copy.sort(key=lambda g: g.fitness, reverse=True)

//     return population_copy[0]

#endif  // UTILS_HPP