#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include <memory>
#include <vector>

#include "neat/experiments/config.hpp"
#include "neat/genotype/connection_gene.hpp"
#include "neat/genotype/genome.hpp"
#include "neat/species.hpp"

class PopulationImpl {
 public:
    PopulationImpl();

    PopulationImpl(const std::shared_ptr<Config>& config, const torch::Device& device);

    std::pair<Genome, int> run();

    void speciate(Genome& genome, int generation);

    void assign_new_model_genomes(Species& species);

    std::vector<Genome> get_genomes_in_species(int species_id);

    std::vector<Genome> set_initial_population();

    static int get_new_innovation_num();

    std::shared_ptr<Config> config;
    std::vector<Species> species;
    std::vector<Genome> population;
    static std::vector<ConnectionGene> current_gen_innovation;
    static int __global_innovation_number;
    torch::Device device;
};

#endif  // POPULATION_HPP_