#ifndef NODE_GENE_HPP_
#define NODE_GENE_HPP_

#include <string>

class NodeGene {
public:
    NodeGene(int node_id, std::string node_type);

    std::string str();

    int id;
    std::string type;
    void* unit;
};

#endif  // NODE_GENE_HPP_