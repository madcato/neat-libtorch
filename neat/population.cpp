#include "neat/population.hpp"

#include <algorithm>
#include <numeric>

#include "neat/crossover.hpp"
#include "neat/mutation.hpp"
#include "neat/utils.hpp"

std::vector<ConnectionGene> PopulationImpl::current_gen_innovation;
int PopulationImpl::__global_innovation_number = 0;

PopulationImpl::PopulationImpl():device(nullptr) {
    assert(false);  // Don't call this method
}

PopulationImpl::PopulationImpl(const std::shared_ptr<Config>& config, const torch::Device& device):config(config), device(device) {
    population = set_initial_population();

    for(auto it = population.begin() ; it != population.end() ; it++) {
        Genome genome = *it;
        speciate(genome, 0);
    }
}

static bool genome_fitness_compare_reverse(const Genome& ele1, const Genome& ele2) {
    return ele1->fitness > ele2->fitness;
}

std::pair<Genome, int> PopulationImpl::run() {
    for(int generation = 0 ; generation < config->number_of_generations() ; generation++) {
        for(auto it = population.begin() ; it != population.end() ; it++) {
            Genome genome = *it;
            genome->fitness = std::max(0.0f, config->fitness_fn(genome));
        }

        Genome best_genome = get_best_genome(population);

        std::vector<int> all_fitnesses;
        std::vector<Species> remaining_species;

        std::vector<std::pair<Species, bool>> stagnations = SpeciesImpl::stagnation(species, generation);
        for(auto it = stagnations.begin() ; it != stagnations.end() ; it++) {
            std::pair<Species, bool> stagnation = *it;
            Species spec = stagnation.first;
            bool is_stagnat = stagnation.second;
            if (is_stagnat == true) {
                auto it_spec = std::find(species.begin(), species.end(), spec);
                species.erase(it_spec);
            } else {
                for(auto it_g = spec->members.begin() ; it_g != spec->members.end() ; it_g++) {
                    Genome g = *it_g;
                    all_fitnesses.push_back(g->fitness);
                }
                remaining_species.push_back(spec);
            }

            int min_fitness = 1000;
            int max_fitness = -1000;
            for(auto it_min = all_fitnesses.begin() ; it_min != all_fitnesses.end() ; it_min++) {
                min_fitness = std::min(*it_min, min_fitness);
                max_fitness = std::max(*it_min, max_fitness);
            }

            int fit_range = std::max(1, max_fitness - min_fitness);
            for(auto it_remain = remaining_species.begin() ; it_remain != remaining_species.end() ; it_remain++) {
                Species spec = *it_remain;
                float n = (float)spec->members.size();
                float avg_species_fitness = 0.0f;
                for(auto it_mem = spec->members.begin() ; it_mem != spec->members.end() ; it_mem++) {
                    Genome genome = *it_mem;
                    avg_species_fitness =+ genome->fitness;
                }
                avg_species_fitness /= n;
                spec->adjusted_fitness = (avg_species_fitness - (float)min_fitness) / (float)fit_range;
            }

            float adj_fitness_sum = 0;
            for(auto it_spec = remaining_species.begin() ; it_spec != remaining_species.end() ; it_spec++) {
                    Species spec = *it_spec;
                    adj_fitness_sum += spec->adjusted_fitness;
            }

            
            int size;
            std::vector<Genome> new_population;
            for(auto it_spec = remaining_species.begin() ; it_spec != remaining_species.end() ; it_spec++) {
                Species spec = *it_spec;
                if(spec->adjusted_fitness > 0) {
                    size = std::max(2, (int)((spec->adjusted_fitness / adj_fitness_sum) * config->population_size()));
                } else {
                    size = 2;
                }

                std::vector<Genome> cur_members = spec->members;
                std::sort(cur_members.begin(), cur_members.end(), genome_fitness_compare_reverse);
                spec->members = {};

                new_population.push_back(cur_members[0]);
                size -= 1;

                int purge_index = (int)(config->percentage_to_save() * cur_members.size());
                purge_index = std::max(2, purge_index);
                cur_members = std::vector<Genome>(cur_members.begin(), cur_members.end() + purge_index);
                
                for(int i = 0 ; i < size ; i++) {
                    Genome parent_1 = choice(cur_members);
                    Genome parent_2 = choice(cur_members);

                    Genome child = crossover(parent_1, parent_2, config, device);
                    mutate(child, config);
                    new_population.push_back(child);
                }
            }
            
            population = new_population;
            PopulationImpl::current_gen_innovation = {};

            for(auto it_gen = population.begin() ; it_gen != population.end() ; it_gen++) {
                Genome genome = *it_gen;
                speciate(genome, generation);
            }

             if(best_genome->fitness >= config->fitness_threshold()) {
                return std::pair<Genome, int>(best_genome, generation);
             }

             if(config->verbose() == true) {
                std::cout << "Finished Generation " << generation << std::endl;
                std::cout << "Best Genome Fitness: " << best_genome->fitness << std::endl;
                std::cout << "Best Genome Length " << best_genome->connection_genes.size() << std::endl;
            }
        }
    }
    return std::pair<Genome, int>(nullptr, 0);
}

void PopulationImpl::speciate(Genome& genome, int generation) {
    for(auto it_spec = species.begin() ; it_spec != species.end() ; it_spec++) {
        Species spec = *it_spec;
        if(spec->species_distance(genome, spec->model_genome) <= config->speciation_threshold()) {
            genome->species = spec->id;
            spec->members.push_back(genome);
            return;
        }
    }

    Species new_species(new SpeciesImpl(species.size(), genome, generation));
    genome->species = new_species->id;
    new_species->members.push_back(genome);
    species.push_back(new_species);
}

void PopulationImpl::assign_new_model_genomes(Species& species) {
    std::vector<Genome> species_pop = get_genomes_in_species(species->id);
    species->model_genome = choice(species_pop);
}

std::vector<Genome> PopulationImpl::get_genomes_in_species(int species_id) {
    std::vector<Genome> g;
    std::copy_if (population.begin(), population.end(), std::back_inserter(g), [species_id](Genome genome) { return genome->species == species_id; });
    return g;
}

std::vector<Genome> PopulationImpl::set_initial_population() {
    std::vector<Genome> pop;
    for(size_t i = 0 ; i < config->population_size() ; i++) {
        Genome new_genome(new GenomeImpl());
        std::vector<NodeGene> inputs;
        std::vector<NodeGene> outputs;
        NodeGene bias;

        for(size_t j = 0 ; j < config->num_inputs() ; j++) {
            NodeGene n = new_genome->add_node_gene("input");
            inputs.push_back(n);
        }

        for(size_t j = 0; j < config->num_outputs() ; j++) {
            NodeGene n = new_genome->add_node_gene("output");
            outputs.push_back(n);
        }

        if(config->use_bias() == true) {
            bias = new_genome->add_node_gene("bias");
        }

        for(auto it_input = inputs.begin() ; it_input != inputs.end() ; it_input++) {
            NodeGene input = *it_input;
            for(auto it_output = outputs.begin() ; it_output != outputs.end() ; it_output++) {
                NodeGene output = *it_output;
                new_genome->add_connection_gene(input->id, output->id);
            }
        }

         if(bias.is_empty() == false) {
             for(auto it_output = outputs.begin() ; it_output != outputs.end() ; it_output++) {
                NodeGene output = *it_output;
                new_genome->add_connection_gene(bias->id, output->id);
            }
         }

        pop.push_back(new_genome);
    }

    return pop;
}

int PopulationImpl::get_new_innovation_num() {
    int ret = PopulationImpl::__global_innovation_number;
    PopulationImpl::__global_innovation_number += 1;
    return ret;
}