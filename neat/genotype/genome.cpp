#include "neat/genotype/genome.hpp"

#include <set>

std::vector<Unit> Genome::order_units(const std::vector<Unit>& units) {
    std::vector<NodeGene> nodes;
    transform(units.begin(), units.end(), back_inserter(nodes), [](const Unit& unit) { return unit.ref_node; });
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
            if (it_u->ref_node == *it_n) {
                ordered_units.push_back(*it_u);
            }
        }
    }

    return ordered_units;
}

std::vector<NodeGene> Genome::get_outputs(const NodeGene& node, const std::vector<NodeGene>& nodes) {
    std::vector<ConnectionGene> temp_genes;
    std::copy_if(connection_genes.begin(), connection_genes.end(), std::back_inserter(temp_genes), [&node](const ConnectionGene& c) { return (c.in_node_id == node.id && c.is_enabled); });
    std::set<int> out_ids;
    transform(temp_genes.begin(), temp_genes.end(), inserter(out_ids, out_ids.begin()), [](const ConnectionGene& gene) { return gene.out_node_id; });
    
    std::vector<NodeGene> temp_nodes;
    std::copy_if(nodes.begin(), nodes.end(), std::back_inserter(temp_nodes), [&out_ids](const NodeGene& unit) { return out_ids.find(unit.id) != out_ids.end(); });
    return temp_nodes;
}

std::vector<NodeGene> Genome::_order_units(const NodeGene& node, 
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