#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "Node.hpp"

class Graph {
private:
    std::vector<Node> nodes;

    IndexesQueue indexes;
    TemporaryIndexesContainer temporaryIndexes;

public:
    explicit Graph(const std::string& filename);

    void addNode(index_t originalIndex, std::unordered_map<index_t, weight_t>& indexMap);
    void linkNodes(Node& node1, Node& node2, weight_t weight);
    void setBValues(method_t method);
    void updateBValues();
    void reset();
    weight_t getResults();

    void runAlgorithm(method_t method, thread_t numberOfThreads);
    void runAlgorithmIteration(IndexesIterator begin, IndexesIterator end);

    std::atomic_flag temporaryIndexesAtomicFlag{ATOMIC_FLAG_INIT};

    Node& getNode(index_t vectorIndex);
    index_t getSize() const;

    // debug
    void print() const;
    bool isValid();
};


#endif //_GRAPH_H_
