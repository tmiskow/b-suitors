#include <cassert>
#include <algorithm>
#include <functional>
#include <iostream>
#include "Node.hpp"

Node::Node(Graph& graph, const index_t originalIndex, const index_t vectorIndex) :
    graph(graph),
    originalIndex(originalIndex),
    vectorIndex(vectorIndex) {}

void Node::addNeighbour(Node& neighbour, const weight_t weight) {
    // TODO: assert that neighbour isn't already in the neighbours vector
    neighbours.emplace_back(weight, neighbour.getOriginalIndex(), neighbour.getVectorIndex());
}

const index_t Node::getOriginalIndex() const {
    return originalIndex;
}

const index_t Node::getVectorIndex() const {
    return vectorIndex;
}

void Node::sortNeighbours() {
    std::sort(neighbours.begin(), neighbours.end(), std::greater<>());
}

void Node::print() const {
    std::cout << "NODE " << getOriginalIndex() << ":\n";

    std::cout << "N: ";
    for (const NodeTuple& neighbourTuple : neighbours) {
        std:: cout << std::get<1>(neighbourTuple) << "(" << std::get<0>(neighbourTuple) << "), ";
    }
    std:: cout << "\n";

    std::cout << "S: ";
    for (const NodeTuple& suitorTuple : suitors) {
        std:: cout << std::get<1>(suitorTuple) << "(" << std::get<0>(suitorTuple) << "), ";
    }
    std:: cout << "\n\n";
}
