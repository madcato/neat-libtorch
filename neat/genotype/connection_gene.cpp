#include "neat/genotype/connection_gene.hpp"

#include "neat/population.hpp"

ConnectionGeneImpl::ConnectionGeneImpl():device(nullptr) {
    assert(false);  // Don't call this method
}

ConnectionGeneImpl::ConnectionGeneImpl(int in_node_id, int out_node_id, bool is_enabled, const torch::Device& device):
                in_node_id(in_node_id), out_node_id(out_node_id), is_enabled(is_enabled), device(device) {
    innov_num = _get_correct_innovation_num();

    set_rand_weight();
}

void ConnectionGeneImpl::set_weight(float new_weight) {
    weight = torch::tensor({new_weight}).to(device);
}

void ConnectionGeneImpl::set_rand_weight() {
    weight = torch::Tensor(at::normal(torch::arange(0, 1).toType(torch::kFloat))).to(device);
}

void ConnectionGeneImpl::set_innov_sum(int num) {
    innov_num = num;
}

int ConnectionGeneImpl::_get_correct_innovation_num() {
    for(auto it = PopulationImpl::current_gen_innovation.begin() ; it != PopulationImpl::current_gen_innovation.end() ; it++) {
        ConnectionGene connect_gene = *it;
        if(this == connect_gene.get()) {
            return connect_gene->innov_num;
        }
    }
    
    PopulationImpl::current_gen_innovation.push_back(ConnectionGene(this));
    return PopulationImpl::get_new_innovation_num();
}

bool ConnectionGeneImpl::operator==(const ConnectionGeneImpl& other) const {
    return ((in_node_id == other.in_node_id) && (out_node_id == other.out_node_id));
}

std::string ConnectionGeneImpl::str() {
    return "In: " + std::to_string(in_node_id) + "\nOut: " + std::to_string(out_node_id) + "\nIs Enabled: " + 
            std::to_string(is_enabled) + "\nInnovation #: " + std::to_string(innov_num) + "\nWeight: " + 
            weight.toString() + "\n";
}
