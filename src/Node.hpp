#ifndef _NODE_H_
#define _NODE_H_


#include <vector>
#include <set>
#include "main.hpp"
#include "Graph.hpp"

class Node {
private:
    Graph& graph;
    const index_t originalIndex;
    const index_t vectorIndex;

    std::vector<NodeTuple> neighbours;
    std::set<NodeTuple> suitors;
    std::vector<NodeTuple>::const_iterator neighboursIterator;


public:
    Node(Graph& graph, index_t originalIndex, index_t vectorIndex);

    void addNeighbour(Node& neighbour, weight_t weight);
    void sortNeighbours();

    const index_t getOriginalIndex() const;
    const index_t getVectorIndex() const;

    // debug
    void print() const;
};


#endif //_NODE_H_
