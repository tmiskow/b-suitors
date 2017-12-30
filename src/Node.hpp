#ifndef _NODE_H_
#define _NODE_H_


#include <vector>
#include <set>
#include <atomic>
#include <mutex>
#include <memory>
#include "main.hpp"
#include "Graph.hpp"

class Node {
private:
    Graph& graph;
    const index_t originalIndex;
    const index_t vectorIndex;

    index_t bValue{0};
    index_t possibleProposals{0};
    std::atomic<index_t> annulledProposals{0};

    std::vector<NodeTuple> neighbours;
    std::set<NodeTuple> suitors;

    std::vector<NodeTuple>::const_iterator neighboursIterator;
    std::unique_ptr<std::mutex> mutexPointer{new std::mutex};

public:
    Node(Graph& graph, index_t originalIndex, index_t vectorIndex);
    Node(Node&& other) noexcept;

    void addNeighbour(Node& neighbour, weight_t weight);
    void sortNeighbours();
    void addSuitor(Node& suitor, weight_t weight);
    void removeWorstSuitor();
    bool needsMoreSuitors();
    NodeTuple getWorstSuitor();
    void annulProposal();
    void reset();
    weight_t getResult();

    const index_t getOriginalIndex() const;
    const index_t getVectorIndex() const;
    index_t getBValue() const;
    std::vector<NodeTuple >::const_iterator& getNeighboursIterator();
    std::mutex& getMutex();
    index_t getPossibleProposals() const;
    const std::vector<NodeTuple>& getNeighbours() const;

    void setBValue(index_t bValue);
    void updateBValue();

    // debug
    void print() const;
    const std::set<NodeTuple>& getSuitors() const;
    bool hasSuitor(index_t originalIndex);
};


#endif //_NODE_H_
