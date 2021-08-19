#include "neat/genotype/genome.hpp"

#include <algorithm>
#include <set>

#include "neat/utils.hpp"

Genome::Genome(const torch::Device& device):device(device) {
}

void Genome::add_connection_mutation() {
    std::vector<NodeGene> potential_inputs;
    std::copy_if (node_genes.begin(), node_genes.end(), std::back_inserter(potential_inputs), [](NodeGene node) { return node->type != "output"; });
    std::vector<NodeGene> potential_outputs;
    std::copy_if (node_genes.begin(), node_genes.end(), std::back_inserter(potential_outputs), [](NodeGene node) { return node->type != "imput" && node->type != "bias"; });

    size_t po_size = potential_outputs.size();
    size_t pi_size = potential_inputs.size();
    if ( po_size != 0 && pi_size != 0) {
        int node_in_id = potential_inputs[rand(0, pi_size)]->id;
        int node_out_id = potential_inputs[rand(0, po_size)]->id;

        if (_is_valid_connection(node_in_id, node_out_id)) {
            add_connection_gene(node_in_id, node_out_id);
        }
    }
}

void Genome::add_node_mutation() {
    NodeGene new_node = add_node_gene("hidden");
    ConnectionGene* existing_connection = _get_rand_connection_gene();

    int weight = 1;
    add_connection_gene(existing_connection->in_node_id, new_node->id, &weight);
    add_connection_gene(new_node->id, existing_connection->out_node_id, &(existing_connection->weight));

    existing_connection->is_enabled = false;
}

int Genome::get_num_excess_genes(Genome& other) {
    int num_excess = 0;

    int max_innov_num = -1;
    for(auto it = other.innov_nums.begin() ; it != other.innov_nums.end() ; it++) {
        max_innov_num = std::max(*it, max_innov_num);
    }
    for(auto c_gene = connection_genes.begin() ; c_gene != connection_genes.end() ; c_gene++) {
        if (c_gene->innov_num > max_innov_num) {
            num_excess += 1;
        }
    }

    int max_node_id = -1;
    for(auto it = other.node_genes.begin() ; it != other.node_genes.end() ; it++) {
        max_node_id = std::max((*it)->id, max_node_id);
    }
    for(auto n = other.node_genes.begin() ; n != other.node_genes.end() ; n++) {
        if ((*n)->id > max_node_id) {
            num_excess += 1;
        }
    }

    return num_excess;
}

int Genome::get_num_disjoint_genes(Genome& other) {
    int num_disjoint = 0;

    int max_innov_num = -1;
    for(auto it = other.innov_nums.begin() ; it != other.innov_nums.end() ; it++) {
        max_innov_num = std::max(*it, max_innov_num);
    }
    
    for(auto c_gene = connection_genes.begin() ; c_gene != connection_genes.end() ; c_gene++) {
        if (c_gene->innov_num <= max_innov_num) {
            if (other.get_connect_gene(c_gene->innov_num) == nullptr) {
                num_disjoint += 1;
            }
        }
    }

    int max_node_id = -1;
    for(auto it = other.node_genes.begin() ; it != other.node_genes.end() ; it++) {
        max_node_id = std::max((*it)->id, max_node_id);
    }

    for(auto n = other.node_genes.begin() ; n != other.node_genes.end() ; n++) {
        if ((*n)->id <= max_node_id) {
            if (other.get_node_gene((*n)->id).is_empty()) {
                num_disjoint += 1;
            }
        }
    }

    return num_disjoint;
}

ConnectionGene* Genome::get_connect_gene(int innov_num) {
    for(auto c_gene = connection_genes.begin() ; c_gene != connection_genes.end() ; c_gene++) {
        if (c_gene->innov_num == innov_num) {
            return &(*c_gene);
        }
    }
return nullptr;
}

NodeGene Genome::get_node_gene(int id) {
    for(auto n = node_genes.begin() ; n != node_genes.end() ; n++) {
        if ((*n)->id == id) {
            return *n;
        }
    }

    return NodeGene();  // nullptr
}

float Genome::get_avg_weight_difference(Genome& other) {
    float weight_difference = 0.0;
    float num_weights = 0.0;

    for(auto c_gene = connection_genes.begin() ; c_gene != connection_genes.end() ; c_gene++) {
        ConnectionGene* matching_gene = other.get_connect_gene(c_gene->innov_num);
        if (matching_gene != nullptr) {
            weight_difference += c_gene->weight[0].item<float>() - matching_gene->weight[0].item<float>();
            num_weights += 1.0;
        }
    }

    if (num_weights == 0.0) {
        num_weights = 1.0;
    }

    return weight_difference / num_weights;
}

std::vector<int> Genome::get_inputs_ids(int node_id) {
    std::vector<int> node_input_ids;
    for(auto c_gene = connection_genes.begin() ; c_gene != connection_genes.end() ; c_gene++) {
        if (c_gene->out_node_id == node_id && c_gene->is_enabled) {
            node_input_ids.push_back(c_gene->in_node_id);
        }
    }

    return node_input_ids;
}

void Genome::add_connection_gene(int node_in_id, int node_out_id, bool is_enable, float* weight) {
    ConnectionGene new_connection_gene = ConnectionGene(node_in_id, node_out_id, is_enable, device);

    if (weight != nullptr) {
        new_connection_gene.set_weight(*weight);
    }

    connection_genes.push_back(new_connection_gene);
    node_ids.insert(node_in_id);
    node_ids.insert(node_out_id);
    innov_nums.insert(new_connection_gene.innov_num);
}

NodeGene Genome::add_node_gene(std::string n_type) {
    int new_id = node_genes.size();
    NodeGene new_gene(new NodeGeneImpl(new_id, n_type));
    node_genes.push_back(new_gene);
    return new_gene; 
}

void Genome::add_connection_copy(const ConnectionGene& copy) {
    ConnectionGene new_c_gene(copy.in_node_id, copy.out_node_id, copy.is_enabled, device);
    new_c_gene.set_weight(copy.weight.item<float>());
    new_c_gene.set_innov_sum(copy.innov_num);

    connection_genes.push_back(new_c_gene);
    node_ids.insert(copy.in_node_id);
    node_ids.insert(copy.out_node_id);
    innov_nums.insert(new_c_gene.innov_num);
}

void Genome::add_node_copy(const NodeGene& copy) {
    node_genes.push_back(NodeGene(new NodeGeneImpl(copy->id, copy->type)));
}

std::vector<ConnectionGene> Genome::get_connections_in(int node_id) {
    std::vector<ConnectionGene> genes; 
    std::copy_if (connection_genes.begin(), connection_genes.end(), std::back_inserter(genes), [&node_id](ConnectionGene gene) { return gene.out_node_id == node_id && gene.is_enabled; });
    return genes;
}

int Genome::_get_rand_node_id() {
    auto it = std::begin(node_ids);
    std::advance(it, rand(0, node_ids.size()));
    return *it;
}

ConnectionGene* Genome::_get_rand_connection_gene() {
    size_t size = connection_genes.size();
    return &connection_genes[rand(0, size)];
}

std::vector<ConnectionGene> Genome::_get_connections_out(int node_id) {
    std::vector<ConnectionGene> genes; 
    std::copy_if (connection_genes.begin(), connection_genes.end(), std::back_inserter(genes), [&node_id](ConnectionGene gene) { return gene.in_node_id == node_id && gene.is_enabled; });
    return genes;
}

bool Genome::creates_cycle(int node_in_id, int node_out_id) {
    if (node_in_id == node_out_id) {
        return true;
    }

    std::set<int> visited = {node_out_id};
    while (true) {
        int num_added = 0;
        for(auto c_gene = connection_genes.begin() ; c_gene != connection_genes.end() ; c_gene++) {
            if (visited.find(c_gene->in_node_id) == visited.end() && visited.find(c_gene->out_node_id) != visited.end()) {
                if (c_gene->out_node_id == node_in_id) {
                    return true;
                } else {
                    visited.insert(c_gene->out_node_id);
                    num_added += 1;
                }
            }
        }

        if (num_added == 0) {
            return false;
        }
    }
}

bool Genome::_is_valid_connection(int node_in_id, int node_out_id) {
    bool does_creates_cycle = creates_cycle(node_in_id, node_out_id);
    bool does_connection_exist = _does_connection_exist(node_in_id, node_out_id);

    return (not does_creates_cycle && !does_connection_exist);
}

bool Genome::_does_connection_exist(int node_1_id, int node_2_id) {
    for(auto c_gene = connection_genes.begin() ; c_gene != connection_genes.end() ; c_gene++) {
        if (c_gene->in_node_id == node_1_id && c_gene->out_node_id == node_2_id) {
            return true;
        } else if (c_gene->in_node_id == node_2_id && c_gene->out_node_id == node_1_id) {
            return true;
        }
    }
    return false;
}

std::vector<NodeGene> Genome::get_outputs(const NodeGene& node, const std::vector<NodeGene>& nodes) {
    std::vector<ConnectionGene> temp_genes;
    std::copy_if(connection_genes.begin(), connection_genes.end(), std::back_inserter(temp_genes), [&node](const ConnectionGene& c) { return (c.in_node_id == node->id && c.is_enabled); });
    std::set<int> out_ids;
    transform(temp_genes.begin(), temp_genes.end(), inserter(out_ids, out_ids.begin()), [](const ConnectionGene& gene) { return gene.out_node_id; });

    std::vector<NodeGene> temp_nodes;
    std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(temp_nodes), [&out_ids](const NodeGene& unit) { return out_ids.find(unit->id) != out_ids.end(); });
    return temp_nodes;
}

std::vector<Unit> Genome::order_units(std::vector<Unit>& units) {
    std::vector<NodeGene> nodes;
    transform(units.begin(), units.end(), back_inserter(nodes), [](const Unit& unit) { return unit->ref_node; });
    std::set<NodeGene> visited;
    std::vector<NodeGene> ordered;

    for(auto it = nodes.begin() ; it != nodes.end() ; it++) {
        if (visited.find(*it) == visited.end()) {
            _order_units(*it, nodes, ordered, visited);
        }
    }

    std::vector<Unit> ordered_units;
    for(auto it_n = ordered.begin() ; it_n != ordered.end() ; it_n++) {
        for(auto it_u = units.begin() ; it_u != units.end() ; it_u++) {
            if ((*it_u)->ref_node == *it_n) {
                ordered_units.push_back(*it_u);
            }
        }
    }

    return ordered_units;
}

void Genome::_order_units(const NodeGene& node, 
                                       std::vector<NodeGene>& nodes,
                                       std::vector<NodeGene>& ordered,
                                       std::set<NodeGene>& visited) {
    visited.insert(node);
    std::vector<NodeGene> outputs = get_outputs(node, nodes);
    for(auto it_n = outputs.begin() ; it_n != outputs.end() ; it_n++) {
        if (visited.find(*it_n) == visited.end()) {
            _order_units(*it_n, nodes, ordered, visited);
        }
    }

    ordered.push_back(node);
}

std::string Genome::str() {
    std::string ret = "Connections:\n\n";
    for(auto c_gene = connection_genes.begin() ; c_gene != connection_genes.end() ; c_gene++) {
        ret += c_gene->str() + "\n";
    }

    ret += "Nodes: \n\n";
    for(auto n_gene = node_genes.begin() ; n_gene != node_genes.end() ; n_gene++) {
        ret += (*n_gene)->str() + "\n";
    }

    return ret;
}
