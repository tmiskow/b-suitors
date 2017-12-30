#include <cassert>
#include <algorithm>
#include <functional>
#include <iostream>
#include <atomic>
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
    neighboursIterator = neighbours.begin();
}

void Node::print() const {
    std::cout << "NODE " << getOriginalIndex() << ":\n";

    std::cout << "b: " << bValue << "\n";
    std::cout << "db: " << annulledProposals << "\n";

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

void Node::setBValue(index_t bValue) {
    this->bValue = bValue;
    possibleProposals = bValue;
    annulledProposals = 0;
}

index_t Node::getBValue() const {
    return bValue;
}

const std::vector<NodeTuple>& Node::getNeighbours() const {
    return neighbours;
}

NodeTuple Node::getWorstSuitor() {
    assert(!suitors.empty());
    return *suitors.begin();
}

bool Node::needsMoreSuitors() {
    return suitors.size() < bValue;
}

void Node::addSuitor(Node& suitor, weight_t weight) {
    suitors.insert(std::make_tuple(weight, suitor.getOriginalIndex(), suitor.getVectorIndex()));
}

void Node::removeWorstSuitor() {
    assert(!suitors.empty());
    suitors.erase(suitors.begin());
}

void Node::annulProposal() {
    annulledProposals++;
}

void Node::updateBValue() {
    possibleProposals = annulledProposals;
    annulledProposals = 0;
}

void Node::reset() {
    neighboursIterator = neighbours.begin();
    suitors.clear();
}

weight_t Node::getResult() {
    weight_t sum = 0;
    for (const NodeTuple& suitorTuple : suitors) {
        sum += std::get<0>(suitorTuple);
    }
    return sum;
}

index_t Node::getPossibleProposals() const {
    return possibleProposals;
}

std::vector<NodeTuple>::const_iterator& Node::getNeighboursIterator() {
    return neighboursIterator;
}

const std::set<NodeTuple>& Node::getSuitors() const {
    return suitors;
}

bool Node::hasSuitor(index_t originalIndex) {
    for (const auto& suitorTuple : suitors) {
        if (std::get<1>(suitorTuple) == originalIndex) {
            return true;
        }
    }

    return false;
}
