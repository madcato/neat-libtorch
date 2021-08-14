#include "neat/genotype/node_gene.hpp"

NodeGene::NodeGene(int node_id, std::string node_type):id(node_id),type(node_type),unit(nullptr) {
}

std::string NodeGene::str() {
    return std::to_string(id) + "-" + type;
}