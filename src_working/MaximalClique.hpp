#pragma once

#include <vector>

namespace HRG_CLIQUE {

/// @brief get the maximal clique of the graph (no need to be maximum)
/// @details greedy algorithm with the order of degree in descending order
/// in time O(N+M)
/// The size of the maximal clique is \Omega(n^{1-\alpha}) with high probability
/// @param adjs the adjacency list of the graph
/// @param N the number of vertices
/// @return the indices of the vertices in the maximal clique
std::vector<int> getMaximalClique(const std::vector<std::vector<int>>& adjs,
                                  const int N);

}  // namespace HRG_CLIQUE
