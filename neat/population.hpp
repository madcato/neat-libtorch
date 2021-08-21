#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <memory>
#include <vector>

#include "neat/experiments/config.hpp"
#include "neat/genotype/genome.hpp"
#include "neat/species.hpp"

class PopulationImpl {
 public:
    PopulationImpl();

    PopulationImpl(const std::shared_ptr<Config>& config);

    void run();

    void speciate(const Genome& genome, int generation);

    void assign_new_model_genomes(const Genome& species);

    std::vector<Genome> get_genomes_in_species(int species_id);

    std::vector<Genome> set_initial_population();

    int get_new_innovation_num();

    std::shared_ptr<Config> config;
    std::vector<Species> species;
    std::vector<Genome> population;
};

#endif  // POPULATION_HPP_