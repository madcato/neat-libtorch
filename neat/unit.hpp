#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <string>
#include <torch/torch.h>

#include "neat/genotype/node_gene.hpp"

class UnitImpl {
public:
    UnitImpl();

    UnitImpl(const NodeGene& ref_node, size_t num_in_features);

    void set_weights(const std::vector<torch::Tensor>& weights);

    torch::nn::Linear build_linear(size_t num_in_features);

    std::string str();

    bool operator==(const UnitImpl& other) const;

    NodeGene ref_node;
    torch::nn::Linear linear = nullptr;
};

SHARED_HELPER(Unit);

#endif  // UNIT_HPP_
