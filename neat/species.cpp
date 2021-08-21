#include "neat/species.hpp"

#include <algorithm>
#include <limits>

SpeciesImpl::SpeciesImpl() {
    assert(false);  // Don't call this method
}

SpeciesImpl::SpeciesImpl(int id, const Genome& genome, int generation):id(id), model_genome(genome), last_improved(generation) {
}

float SpeciesImpl::species_distance(Genome& genome_1, Genome& genome_2) {
    float C1 = 1.0;
    float C2 = 1.0;
    float C3 = 0.5;
    float N = 1;

    int num_excess = genome_1->get_num_excess_genes(genome_2);
    int num_disjoint = genome_1->get_num_disjoint_genes(genome_2);
    int avg_weight_difference = genome_1->get_avg_weight_difference(genome_2);

    float distance = (C1 * num_excess) / N;
    distance += (C2 * num_disjoint) / N;
    distance += C3 * avg_weight_difference;
    return distance;
}

static bool genome_fitness_compare(const Genome& ele1, const Genome& ele2) {
    return ele1->fitness < ele2->fitness;
}

static bool species_fitness_compare(const Species& ele1, const Species& ele2) {
    return ele1->fitness < ele2->fitness;
}

std::vector<std::pair<Species, bool>> SpeciesImpl::stagnation(std::vector<Species>& species, int generation) {
    std::vector<Species> species_data;
    std::vector<std::pair<Species, bool>> result;

    for(auto s = species.begin() ; s != species.end() ; s++) {
        Species spec = *s;
        int prev_fitness = -std::numeric_limits<int>::max();
        if (spec->fitness_history.size() > 0) {
            int max = -std::numeric_limits<int>::max();
            for(auto it = spec->fitness_history.begin() ; it != spec->fitness_history.end() ; it++) {
                max = std::max(*it, max);
            }
            prev_fitness = max;
        }

        Genome genome = *(std::max_element(spec->members.begin(), spec->members.end(), genome_fitness_compare));
        spec->fitness = genome->fitness;
        spec->fitness_history.push_back(spec->fitness);
        spec->adjusted_fitness = 0.0f;

        if (prev_fitness == 0 || spec->fitness > prev_fitness) {
            spec->last_improved = generation;
        }

        species_data.push_back(spec);
    }
    
    std::sort(species_data.begin(),species_data.end(), species_fitness_compare);

    
    // std::vector<int> species_fitnesses;
    int num_non_stagnant = species_data.size();
    int i = 0;
    for(auto spec_data = species_data.begin(); spec_data != species_data.end() ; spec_data++) {
        int stagnant_time = generation - (*spec_data)->last_improved;
        bool is_stagnant = false;
        if(num_non_stagnant > 1) {
            is_stagnant = stagnant_time >= 10;
        }

        if(species_data.size() - i <= 1) {
            is_stagnant = false;
        }
        if(species_data.size() - i <= 1) {
            is_stagnant = false;
        }

        if(is_stagnant == true) {
            num_non_stagnant -= 1;
        }

        result.push_back(std::make_pair(*spec_data, is_stagnant));
        // species_fitnesses.push_back(spec_dat->.fitness)
        i++;
    }
    return result;
}

bool SpeciesImpl::operator==(const SpeciesImpl& other) const {
    return this == &other;
}