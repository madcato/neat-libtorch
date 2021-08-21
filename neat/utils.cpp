#include "neat/utils.hpp"

#include <cstdlib>
#include <torch/torch.h>

float rand_uni_val() {
    return torch::rand({1})[0].item<float>();
}

bool rand_bool() {
    return rand_uni_val() <= 0.5;
}

int rand(int min, int max) {
    return min + (std::rand() % static_cast<int>(max - min + 1));
}

float rand(float min, float max) {
    return min + static_cast <float>(std::rand() / static_cast<float>(RAND_MAX / max - min ));
}

static bool genome_fitness_compare_reverse(const Genome& ele1, const Genome& ele2) {
    return ele1->fitness > ele2->fitness;
}

Genome get_best_genome(const std::vector<Genome>& population) {
    std::vector<Genome> population_copy = population;
    std::sort(population_copy.begin(), population_copy.end(), genome_fitness_compare_reverse);
    
    return *population_copy.begin();
}
