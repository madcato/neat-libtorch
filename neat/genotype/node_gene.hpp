#ifndef NODE_GENE_HPP_
#define NODE_GENE_HPP_

using namespace std;

class NodeGene {
public:
    NodeGene(size_t node_id, string node_type);

    string str();

    size_t id;
    string type;
    void* unit;
};

#endif  // NODE_GENE_HPP_