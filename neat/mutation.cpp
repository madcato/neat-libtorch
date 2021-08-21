#include "neat/mutation.hpp"

#include "neat/genotype/connection_gene.hpp"
#include "neat/utils.hpp"

void mutate(Genome& genome, std::shared_ptr<Config> config) {
    if (rand_uni_val() < config->connection_mutation_rate()) {
        for(auto it = genome->connection_genes.begin() ; it != genome->connection_genes.end() ; it++) {
            ConnectionGene c_gene = *it;
            if (rand_uni_val() < config->connection_mutation_rate()) {
                float perturb = rand_uni_val() * choice<int>(std::vector<int>({1, -1}));
                c_gene->weight += perturb;
            } else {
                c_gene->set_rand_weight();
            }
        }
    }

    if(rand_uni_val() < config->add_connection_mutation_rate()) {
        genome->add_node_mutation();
    }

    if(rand_uni_val() < config->add_connection_mutation_rate()) {
        genome->add_connection_mutation();
    }
}