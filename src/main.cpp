#include <iostream>
#include <cassert>
#include <chrono>
#include <unordered_set>
#include "main.hpp"
#include "Graph.hpp"

int main(int argc, char* argv[]) {
    assert(argc == 4);
    const unsigned int numberOfThreads = static_cast<unsigned int>(std::stoul(argv[1]));
    const std::string filename = argv[2];
    const method_t bLimit = static_cast<method_t>(std::stoul(argv[3]));

    auto loadingStartTime = std::chrono::system_clock::now();
    Graph graph(filename);
    auto loadingEndTime = std::chrono::system_clock::now();
    std::chrono::duration<double> loadingDuration = loadingEndTime - loadingStartTime;
//    std::cout << "Graph loading time: " << loadingDuration.count() << " seconds\n";
    auto algorithmStartTime = std::chrono::system_clock::now();
    for (method_t method = 0; method <= bLimit; ++method) {
        std::vector<index_t> indexes;
        std::unordered_set<index_t> temporaryIndexes;
        for (index_t i = 0; i < graph.getSize(); i++) {
            indexes.push_back(i);
        }

        graph.setBValues(method);
        while (!indexes.empty()) {
//            graph.print();

            for (index_t index : indexes) {
                Node& node = graph.getNode(index);
                index_t foundPartners = 0;

                auto& iterator = node.getNeighboursIterator();
                while (foundPartners < node.getPossibleProposals() && iterator != node.getNeighbours().end()) {
                    weight_t candidateWeight;
                    index_t candidateOriginalIndex, candidateVectorIndex;

                    NodeTuple candidateTuple = *iterator;
                    std::tie(candidateWeight, candidateOriginalIndex, candidateVectorIndex) = candidateTuple;
                    Node& candidate = graph.getNode(candidateVectorIndex);

                    if (candidate.getBValue() > 0) {

                        // lock candidate
                        if (candidate.needsMoreSuitors()) {
                            foundPartners++;
                            candidate.addSuitor(node, candidateWeight);
                        } else {
                            NodeTuple competitorTuple = candidate.getWorstSuitor();
                            NodeTuple nodeTuple = std::make_tuple(candidateWeight, node.getOriginalIndex(), node.getVectorIndex());
                            if (nodeTuple > competitorTuple) {
                                index_t competitorVectorIndex = std::get<2>(competitorTuple);
                                Node& competitor = graph.getNode(competitorVectorIndex);

                                // make atomic
                                competitor.annulProposal();
                                candidate.removeWorstSuitor();

                                // lock temporary indexes
                                temporaryIndexes.insert(competitorVectorIndex);
                                // unlock temporary indexes

                                foundPartners++;
                                candidate.addSuitor(node, candidateWeight);
                            }
                        }
                        // unlock candidate
                    }

                    ++iterator;
                }
            }

            indexes = std::vector<index_t>(temporaryIndexes.begin(), temporaryIndexes.end());
            temporaryIndexes.clear();
            graph.updateBValues();
        }
//        graph.print();
        assert(graph.isValid());
        std::cout << graph.getResults() << '\n';
        graph.reset();
    }

    auto algorithmEndTime = std::chrono::system_clock::now();
    std::chrono::duration<double> algorithmDuration = algorithmEndTime - algorithmStartTime;
//    std::cout << "Algorithm time: " << algorithmDuration.count() << " seconds\n";

}