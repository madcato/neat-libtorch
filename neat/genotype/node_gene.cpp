#include <string>

#include "node_gene.hpp"

NodeGene::NodeGene(size_t node_id, string node_type):id(node_id),type(node_type),unit(nullptr) {
}

string NodeGene::str() {
    return to_string(id) + "-" + type;
}