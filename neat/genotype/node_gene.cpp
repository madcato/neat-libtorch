#include "neat/genotype/node_gene.hpp"

NodeGeneImpl::NodeGeneImpl() {
}

NodeGeneImpl::NodeGeneImpl(int node_id, std::string node_type):id(node_id),type(node_type),unit(nullptr) {
}

bool NodeGeneImpl::operator==(const NodeGeneImpl& other) const {
    return (id == other.id && type == other.type && unit == other.unit);
}

bool NodeGeneImpl::operator<(const NodeGeneImpl& other) const {
    return id < other.id;
}

std::string NodeGeneImpl::str() {
    return std::to_string(id) + "-" + type;
}