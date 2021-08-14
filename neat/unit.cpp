#include "neat/unit.hpp"
#include <vector>

Unit::Unit(const NodeGene& ref_node, size_t num_in_features):ref_node(ref_node) {
    linear = register_module("linear", build_linear(num_in_features));
}

void Unit::set_weights(const torch::Tensor& weights) {
    if ((ref_node.type != "imput") && (ref_node.type != "bias")) {
        torch::Tensor uweights = torch::cat(weights).unsqueeze(0);
        std::vector<torch::Tensor> param_vec = linear->parameters();
        for(auto it = param_vec.begin(); it != param_vec.end(); ++it) {
            it->set_data(uweights);
        }
    }
}

torch::nn::Linear Unit::build_linear(size_t num_in_features) {
    if ((ref_node.type != "imput") && (ref_node.type != "bias")) {
        return nullptr;
    }

    return torch::nn::Linear(torch::nn::LinearOptions(num_in_features, 1).bias(false));
}

std::string Unit::str() {
    return "Reference Node: " + ref_node.str() + "\n";
}