#ifndef GENOME_HPP_
#define GENOME_HPP_

#include <vector>
#include <torch/torch.h>

#include "neat/unit.hpp"
#include "neat/genotype/connection_gene.hpp"
#include "neat/genotype/node_gene.hpp"

class Genome;

class GenomeImpl {
public:
    GenomeImpl();

    GenomeImpl(const torch::Device& device);

    void add_connection_mutation();

    void add_node_mutation();

    int get_num_excess_genes(Genome& other);

    int get_num_disjoint_genes(Genome& other);

    ConnectionGene get_connect_gene(int innov_num);

    NodeGene get_node_gene(int id);

    float get_avg_weight_difference(Genome& other);

    std::vector<int> get_inputs_ids(int node_id);

    void add_connection_gene(int node_in_id, int node_out_id, bool is_enable = true, float* weight = nullptr);

    NodeGene add_node_gene(std::string n_type);

    void add_connection_copy(const ConnectionGene& copy);

    void add_node_copy(const NodeGene& copy);

    std::vector<ConnectionGene> get_connections_in(int node_id);

    std::vector<Unit> order_units(std::vector<Unit>& units);

    std::vector<NodeGene> get_outputs(const NodeGene& node, const std::vector<NodeGene>& nodes);

    std::string str();

    int _get_rand_node_id();

    ConnectionGene _get_rand_connection_gene();

    std::vector<ConnectionGene> _get_connections_out(int node_id);

    bool creates_cycle(int node_in_id, int node_out_id);

    bool _is_valid_connection(int node_in_id, int node_out_id);

    bool _does_connection_exist(int node_1_id, int node_2_id);

    void _order_units(const NodeGene& node, 
                      std::vector<NodeGene>& nodes,
                      std::vector<NodeGene>& ordered,
                      std::set<NodeGene>& visited);

    std::vector<ConnectionGene> connection_genes;
    std::vector<NodeGene> node_genes;
    std::set<int> node_ids;
    std::set<int> innov_nums;
    int fitness;
    int adjusted_fitness;
    // species
    torch::Device device;
};

SHARED_HELPER(Genome);

#endif  // GENOME_HPP_