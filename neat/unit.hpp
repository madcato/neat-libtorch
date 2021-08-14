#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <string>
#include <torch/torch.h>

#include "neat/genotype/node_gene.hpp"

class Unit: public torch::nn::Module {
public:
    Unit(const NodeGene& ref_node, size_t num_in_features);

    void set_weights(const torch::Tensor& weights);

    torch::nn::Linear build_linear(size_t num_in_features);

    std::string str();

    NodeGene ref_node;
    torch::nn::Linear linear = nullptr;
};

#endif  // UNIT_HPP_
