#include "neat/genotype/node_gene.hpp"

NodeGene::NodeGene(int node_id, std::string node_type):id(node_id),type(node_type),unit(nullptr) {
}

bool NodeGene::operator==(const NodeGene& other) const {
    return (id == other.id && type == other.type && unit == other.unit);
}

bool NodeGene::operator<(const NodeGene& other) const {
    return id < other.id;
}

std::string NodeGene::str() {
    return std::to_string(id) + "-" + type;
}