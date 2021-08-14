#ifndef GENOME_HPP_
#define GENOME_HPP_

#include <vector>

#include "neat/unit.hpp"
#include "neat/genotype/connection_gene.hpp"

class Genome {
public:
    std::vector<Unit> order_units(const std::vector<Unit>& units);

    std::vector<NodeGene> get_outputs(const NodeGene& node, const std::vector<NodeGene>& nodes);

private:
    std::vector<NodeGene> _order_units(const NodeGene& node, 
                                   std::vector<NodeGene>& nodes,
                                   std::vector<NodeGene>& ordered,
                                   std::set<NodeGene>& visited);


    std::vector<ConnectionGene> connection_genes;
};

#endif  // GENOME_HPP_