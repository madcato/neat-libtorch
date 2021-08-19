#ifndef CONNECTION_GENE_HPP_
#define CONNECTION_GENE_HPP_

#include <string>
#include "torch/torch.h"

#include "neat/shared_helper.hpp"

class ConnectionGeneImpl {
public:
    ConnectionGeneImpl();

    ConnectionGeneImpl(int in_node_id, int out_node_id, bool is_enabled, const torch::Device& device);

    void set_weight(float new_weight);

    void set_rand_weight();

    void set_innov_sum(int num);

    int _get_correct_innovation_num();

    bool operator==(const ConnectionGeneImpl& other) const;

    std::string str();

    int in_node_id;
    int out_node_id;
    bool is_enabled;
    int innov_num;
    torch::Tensor weight = {};
    torch::Device device;
};

SHARED_HELPER(ConnectionGene);

#endif  // CONNECTION_GENE_HPP_