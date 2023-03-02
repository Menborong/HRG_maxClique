#pragma once

#include <vector>

namespace HRG_CLIQUE{

    // get the maximal clique of the graph
    // using greedy algorithm with the order of degree in descending order
    // time complexity: O(N+M)
    std::vector<int> getMaximalClique(std::vector<std::vector<int>>& adjs, int N);

    // get the new vertex set after reduction
    // by deleting all vertices with degree lower than redSize
    // until there is no such a vertex.
    // time complexity: O(N+M)
    std::vector<int> reduction(std::vector<std::vector<int>>& adjs, int N, int redSize);

}