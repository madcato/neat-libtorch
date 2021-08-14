#include "neat/genotype/connection_gene.hpp"

ConnectionGene::ConnectionGene(int in_node_id, int out_node_id, bool is_enabled, const torch::Device& device):
                in_node_id(in_node_id), out_node_id(out_node_id), is_enabled(is_enabled), device(device) {
    innov_num = _get_correct_innovation_num();

    set_rand_weight();
}

void ConnectionGene::set_weight(float new_weight) {
    weight = torch::tensor({new_weight}).to(device);
}

void ConnectionGene::set_rand_weight() {
    weight = torch::Tensor(at::normal(torch::arange(0, 1).toType(torch::kFloat))).to(device);
}

void ConnectionGene::set_innov_sum(int num) {
    innov_num = num;
}

int ConnectionGene::_get_correct_innovation_num() {
    // TODO: translate this
    // # This method keeps track of a generation's innovations
    // for connect_gene in neat.population.Population.current_gen_innovation:
    //         if self == connect_gene:
    //             return connect_gene.innov_num
    //     # Is new innovation
    //     neat.population.Population.current_gen_innovation.append(self)
    //     return neat.population.Population.get_new_innovation_num()
    return 0;
}

bool ConnectionGene::operator==(const ConnectionGene& other) const {
    return ((in_node_id == other.in_node_id) && (out_node_id == other.out_node_id));
}

std::string ConnectionGene::str() {
    return "In: " + std::to_string(in_node_id) + "\nOut: " + std::to_string(out_node_id) + "\nIs Enabled: " + 
            std::to_string(is_enabled) + "\nInnovation #: " + std::to_string(innov_num) + "\nWeight: " + 
            weight.toString() + "\n";
}
