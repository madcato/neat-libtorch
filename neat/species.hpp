#ifndef SPECIES_HPP_
#define SPECIES_HPP_

#include <vector>
#include <utility>

#include "neat/genotype/genome.hpp"

class Species;

class SpeciesImpl {
 public:
    SpeciesImpl();
    
    SpeciesImpl(int id, const Genome& genome, int generation);

    static float species_distance(Genome& genome_1, Genome& genome_2);

    static std::vector<std::pair<Species, bool>> stagnation(std::vector<Species>& species, int generation);

    bool operator==(const SpeciesImpl& other) const;

    int id;
    Genome model_genome;
    std::vector<Genome> members;
    std::vector<int> fitness_history;
    int fitness;
    float adjusted_fitness;
    int last_improved;
};

SHARED_HELPER(Species);

#endif  // SPECIES_HPP_