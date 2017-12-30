#include <fstream>
#include <unordered_map>
#include <sstream>
#include <cassert>
#include <iostream>
#include <unordered_set>
#include <thread>
#include "Graph.hpp"
#include "blimit.hpp"

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

void Graph::setBValues(method_t method) {
    for (Node& node : nodes) {
        index_t bValue = bvalue(method, node.getOriginalIndex());
        node.setBValue(bValue);
    }
}

void Graph::updateBValues() {
    for (Node& node : nodes) {
        node.updateBValue();
    }
}

void Graph::reset() {
    for (Node& node : nodes) {
        node.reset();
    }
}

weight_t Graph::getResults() {
    weight_t sum = 0;
    for (Node& node : nodes) {
        sum += node.getResult();
    }
    return sum/2;
}

bool Graph::isValid() {
    for (Node& node : nodes) {
        for (const auto& suitorTuple : node.getSuitors()) {
            Node& suitor = getNode(std::get<2>(suitorTuple));
            if (suitor.hasSuitor(node.getOriginalIndex()));
        }
    }

    return true;
}

void Graph::runAlgorithm(method_t method, thread_t possibleThreads) {
    indexes.clear();
    temporaryIndexes.clear();

    for (index_t i = 0; i < getSize(); i++) {
        indexes.push_back(i);
    }

    setBValues(method);
    while (!indexes.empty()) {
        thread_t numberOfThreads = std::min(possibleThreads, indexes.size());
        index_t numberOfNodes = indexes.size() / numberOfThreads;

        std::vector<std::thread> threads;

        auto begin = indexes.begin();
        auto end = indexes.begin();
        for (index_t i = 1; i < numberOfThreads; i++) {
            begin = end;
            end = begin + numberOfNodes;
            threads.emplace_back([this, begin, end] {
                this->runAlgorithmIteration(begin, end);
            });
        }
        begin = end;
        end = indexes.end();
        runAlgorithmIteration(begin, end);

        for (auto& thread : threads) {
            thread.join();
        }

        indexes = std::vector<index_t>(temporaryIndexes.begin(), temporaryIndexes.end());
        temporaryIndexes.clear();
        updateBValues();
    }
}

void Graph::runAlgorithmIteration(IndexesIterator begin, IndexesIterator end) {
    TemporaryIndexesContainer localIndexes;

    for (auto indexIterator = begin; indexIterator != end; ++ indexIterator) {
        Node& node = getNode(*indexIterator);
        index_t foundPartners = 0;

        auto& neighboursIterator = node.getNeighboursIterator();
        while (foundPartners < node.getPossibleProposals() && neighboursIterator != node.getNeighbours().end()) {
            weight_t candidateWeight;
            index_t candidateOriginalIndex, candidateVectorIndex;

            NodeTuple candidateTuple = *neighboursIterator;
            std::tie(candidateWeight, candidateOriginalIndex, candidateVectorIndex) = candidateTuple;
            Node& candidate = getNode(candidateVectorIndex);

            if (candidate.getBValue() > 0) {
                candidate.lock();
                {
                    if (candidate.needsMoreSuitors()) {
                        foundPartners++;
                        candidate.addSuitor(node, candidateWeight);
                    } else {
                        const NodeTuple& competitorTuple = candidate.getWorstSuitor();
                        NodeTuple nodeTuple = std::make_tuple(candidateWeight, node.getOriginalIndex(), node.getVectorIndex());
                        if (nodeTuple > competitorTuple) {
                            index_t competitorVectorIndex = std::get<2>(competitorTuple);
                            Node& competitor = getNode(competitorVectorIndex);
                            competitor.annulProposal();
                            candidate.removeWorstSuitor();
                            localIndexes.insert(competitorVectorIndex);
                            foundPartners++;
                            candidate.addSuitor(node, candidateWeight);
                        }
                    }
                }
                candidate.unlock();
            }

            ++neighboursIterator;
        }
    }

    {
        while (temporaryIndexesAtomicFlag.test_and_set(std::memory_order_acquire))  {
            // spin
        }
        for (index_t index : localIndexes) {
            temporaryIndexes.insert(index);
        }
        temporaryIndexesAtomicFlag.clear(std::memory_order_release);
    }
}
