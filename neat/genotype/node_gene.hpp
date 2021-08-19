#ifndef NODE_GENE_HPP_
#define NODE_GENE_HPP_

#include <string>

#include "neat/shared_helper.hpp"

class NodeGeneImpl {
public:
    NodeGeneImpl();

    NodeGeneImpl(int node_id, std::string node_type);

    bool operator==(const NodeGeneImpl& other) const;
    
    bool operator<(const NodeGeneImpl& other) const;

    std::string str();

    int id;
    std::string type;
    void* unit;
};

SHARED_HELPER(NodeGene);

#endif  // NODE_GENE_HPP_