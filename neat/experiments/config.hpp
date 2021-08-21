#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>
#include <torch/torch.h>

#include "neat/genotype/genome.hpp"

class Config {
public:
    virtual torch::Device device() = 0;

    virtual bool verbose() = 0;

    virtual size_t num_inputs() = 0;

    virtual size_t num_outputs() = 0;

    virtual bool use_bias() = 0;

    virtual std::string activation() = 0;

    virtual float scale_activation() = 0;

    virtual float fitness_threshold() = 0;

    virtual size_t population_size() = 0;

    virtual int number_of_generations() = 0;

    virtual float speciation_threshold() = 0;

    virtual float connection_mutation_rate() = 0;

    virtual float connection_permutation_rate() = 0;
    
    virtual float add_node_mutation_rate() = 0;
    
    virtual float add_connection_mutation_rate() = 0;
    
    virtual float crossover_reenable_connection_gene_rate() = 0;

    virtual float percentage_to_save() = 0;

    virtual float fitness_fn(const Genome& genome) = 0;

    virtual float get_preds_and_labels(const Genome& genome) = 0;

};


#endif  // CONFIG_HPP_