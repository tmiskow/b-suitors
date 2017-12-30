#include <fstream>
#include <unordered_map>
#include <sstream>
#include <cassert>
#include <iostream>
#include "Graph.hpp"

Graph::Graph(const std::string& filename) {
    std::ifstream file;
    std::unordered_map<index_t, weight_t> indexMap;

    file.open(filename);

    std::string line;
    while (std::getline(file, line)) {
        if (line.at(0) != '#') {
            weight_t weight;
            index_t originalIndex1, originalIndex2;

            std::stringstream(line) >> originalIndex1 >> originalIndex2 >> weight;

            if (indexMap.count(originalIndex1) == 0) {
                addNode(originalIndex1, indexMap);
            }

            if (indexMap.count(originalIndex2) == 0) {
                addNode(originalIndex2, indexMap);
            }

            index_t vectorIndex1 = indexMap[originalIndex1];
            index_t vectorIndex2 = indexMap[originalIndex2];
            linkNodes(getNode(vectorIndex1), getNode(vectorIndex2), weight);
        }
    }

    file.close();

    for (Node& node : nodes) {
        node.sortNeighbours();
    }
}

void Graph::addNode(index_t originalIndex, std::unordered_map<index_t, weight_t>& indexMap) {
    assert(indexMap.count(originalIndex) == 0);
    index_t vectorIndex = nodes.size();
    nodes.emplace_back(*this, originalIndex, vectorIndex);
    indexMap.insert(std::make_pair(originalIndex, vectorIndex));
}

Node& Graph::getNode(index_t vectorIndex) {
    assert(vectorIndex < nodes.size());
    return  nodes[vectorIndex];
}

void Graph::linkNodes(Node& node1, Node& node2, weight_t weight) {
    node1.addNeighbour(node2, weight);
    node2.addNeighbour(node1, weight);
}

void Graph::print() const {
    if (DEBUG_MODE) {
        std::cout << "\n";
        for (const Node& node : nodes) {
            node.print();
        }
    }
}

index_t Graph::getSize() const {
    return nodes.size();
}
