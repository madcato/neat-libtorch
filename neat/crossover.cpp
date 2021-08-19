#include "neat/crossover.hpp"

#include "neat/utils.hpp"

Genome crossover(const Genome& genome_1, const Genome& genome_2, const std::shared_ptr<Config>& config, const torch::Device& device) {
    Genome child = Genome(new GenomeImpl(device));

    std::pair<Genome, Genome> parents =  order_parents(genome_1, genome_2);
    Genome best_parent = parents.first;
    Genome other_parent = parents.second;

    ConnectionGene child_connection_gene;

    for(auto c_gene = best_parent->connection_genes.begin() ; c_gene != best_parent->connection_genes.end() ; c_gene++) {
        ConnectionGene matching_gene = other_parent->get_connect_gene((*c_gene)->innov_num);

       if (!matching_gene.is_empty()) {
            if (rand_bool()) {
                // TODO: implement this
                // child_connection_gene = deepcopy(c_gene)
            } else {
                // child_connection_gene = deepcopy(matching_gene)
            }
        } else {
            // child_connection_gene = deepcopy(c_gene)
        }

        if (child_connection_gene->is_enabled) {
            bool is_reenabeled = rand_uni_val() <= config->crossover_reenable_connection_gene_rate();
            bool enabled_in_best_parent = best_parent->get_connect_gene(child_connection_gene->innov_num)->is_enabled;

            if(is_reenabeled || enabled_in_best_parent) {
                child_connection_gene->is_enabled = true;
            }
        }

        child->add_connection_copy(child_connection_gene);
    }

    NodeGene child_gene;
    for(auto c_gene = best_parent->node_genes.begin() ; c_gene != best_parent->node_genes.end() ; c_gene++) {
        NodeGene matching_gene = other_parent->get_node_gene((*c_gene)->id);

        if (matching_gene.is_empty()) {
            if (rand_bool()) {
                // TODO: implement this
                // child_gene = deepcopy(n_gene)
            } else {
                // child_gene = deepcopy(matching_gene)
            }
        } else {
            // child_gene = deepcopy(n_gene);
        }

        child->add_node_copy(child_gene);
    }

    return child;
}

std::pair<Genome, Genome> order_parents(const Genome& parent_1, const Genome& parent_2) {
    Genome best_parent = parent_1;
    Genome other_parent = parent_2;

    size_t len_parent_1 = parent_1->connection_genes.size();
    size_t len_parent_2 = parent_2->connection_genes.size();

    if (parent_1->fitness == parent_2->fitness) {
        if (len_parent_1 == len_parent_2) {
            if (rand_bool()) {
                best_parent = parent_2;
                other_parent = parent_1;
            }
        } else if (len_parent_2 < len_parent_1) {
            best_parent = parent_2;
            other_parent = parent_1;
        }
    } else if (parent_2->fitness > parent_1->fitness) {
        best_parent = parent_2;
        other_parent = parent_1;
    }

    return std::pair<Genome, Genome>(best_parent, other_parent);
}
