#ifndef UNIT_HPP_
#define UNIT_HPP_

#include "genotype/node_gene.hpp"

using namespace std;

class Unit: public torch::nn::Module {
    Unit(const NodeGene& ref_node, size_t num_in_features);

    void set_weights(const torch::Tensor& weights);

    torch::nn::Linear build_linear(size_t num_in_features);

    string str();

private:
    NodeGene ref_node;
    torch::nn::Linear linear = nullptr;
};

#endif  // UNIT_HPP_
