#ifndef _TYPES_H_
#define _TYPES_H_

#include <tuple>
#include <vector>
#include <unordered_set>

#ifdef NDEBUG
    const bool DEBUG_MODE = false;
#else
    const bool DEBUG_MODE = true;
#endif

class Graph;
class Node;

using method_t = unsigned int;
using index_t = unsigned long;
using weight_t = unsigned long;

// (weight, original index, vector index)
using NodeTuple = std::tuple<weight_t , index_t, index_t>;

using IndexesQueue = std::vector<index_t>;
using TemporaryIndexesContainer = std::unordered_set<index_t>;
using IndexesIterator = std::vector<index_t>::const_iterator;

#endif //_TYPES_H_
