#ifndef UTILS_HPP
#define UTILS_HPP

float rand_uni_val();
bool rand_bool();
int rand(int min, int max);

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