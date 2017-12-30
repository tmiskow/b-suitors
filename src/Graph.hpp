#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "Node.hpp"

class Graph {
private:
    std::vector<Node> nodes;

public:
    explicit Graph(const std::string& filename);

    void addNode(index_t originalIndex, std::unordered_map<index_t, weight_t>& indexMap);
    void linkNodes(Node& node1, Node& node2, weight_t weight);

    Node& getNode(index_t vectorIndex);
    index_t getSize() const;

    // debug
    void print() const;
};


#endif //_GRAPH_H_
