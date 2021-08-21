#include "neat/population.hpp"

PopulationImpl::PopulationImpl() {
    assert(false);  // Don't call this method
}

PopulationImpl::PopulationImpl(const std::shared_ptr<Config>& config):config(config) {
    population = set_initial_population();

    for(auto it = population.begin() ; it != population.end() ; it++) {
        Genome genome = *it;
        speciate(genome, 0);
    }
}

void PopulationImpl::run() {

}

void PopulationImpl::speciate(const Genome& genome, int generation) {

}

void PopulationImpl::assign_new_model_genomes(const Genome& species) {

}

std::vector<Genome> PopulationImpl::get_genomes_in_species(int species_id) {
    return {};
}

std::vector<Genome> PopulationImpl::set_initial_population() {
    return {};
}

int PopulationImpl::get_new_innovation_num() {
    return 0;
}