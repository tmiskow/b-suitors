#include <iostream>
#include <cassert>
#include <chrono>
#include <unordered_set>
#include "main.hpp"
#include "Graph.hpp"

int main(int argc, char* argv[]) {
    assert(argc == 4);
    const thread_t numberOfThreads = static_cast<thread_t>(std::stoul(argv[1]));
    const std::string filename = argv[2];
    const method_t bLimit = static_cast<method_t>(std::stoul(argv[3]));

    auto loadingStartTime = std::chrono::system_clock::now();
    Graph graph(filename);
    auto loadingEndTime = std::chrono::system_clock::now();
    std::chrono::duration<double> loadingDuration = loadingEndTime - loadingStartTime;
//    std::cout << "Graph loading time: " << loadingDuration.count() << " seconds\n";
    auto algorithmStartTime = std::chrono::system_clock::now();
    for (method_t method = 0; method <= bLimit; ++method) {
        graph.runAlgorithm(method, numberOfThreads);
        assert(graph.isValid());
        std::cout << graph.getResults() << '\n';
        graph.reset();
    }

    auto algorithmEndTime = std::chrono::system_clock::now();
    std::chrono::duration<double> algorithmDuration = algorithmEndTime - algorithmStartTime;
//    std::cout << "Algorithm time: " << algorithmDuration.count() << " seconds\n";

}