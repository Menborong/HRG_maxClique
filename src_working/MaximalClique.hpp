#pragma once

#include <vector>

namespace HRG_CLIQUE {

// get the maximal clique of the graph (no need to be maximum)
// using greedy algorithm with the order of degree in descending order
// time complexity: O(N+M)
std::vector<int> getMaximalClique(const std::vector<std::vector<int>>& adjs,
                                  const int N);

}  // namespace HRG_CLIQUE
