#include <iostream>
#include <cassert>
#include <chrono>
#include "main.hpp"
#include "Graph.hpp"

int main(int argc, char* argv[]) {
    assert(argc == 4);
    unsigned int numberOfThreads = static_cast<unsigned int>(std::stoul(argv[1]));
    std::string filename = argv[2];
    method_t bLimit = static_cast<method_t>(std::stoul(argv[3]));


    auto start = std::chrono::system_clock::now();
    Graph graph(filename);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Graph loading time: " << diff.count() << " seconds\n";

    graph.print();
}