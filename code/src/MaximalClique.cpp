#include "MaximalClique.hpp"

#include <vector>

std::vector<int> HRG_CLIQUE::getMaximalClique(
    const std::vector<std::vector<int>>& adjs, const int N) {
    // sort the vertices by degree in descending order using counting sort
    // time complexity: O(N)
    std::vector<std::vector<int>> degrees(N + 1);
    for (int i = 1; i <= N; i++) {
        degrees[adjs[i].size()].push_back(i);
    }
    std::vector<int> sorted;
    for (int i = N; i >= 0; i--) {
        for (int j = 0; j < degrees[i].size(); j++) {
            sorted.push_back(degrees[i][j]);
        }
    }

    // choose vertices greedily
    std::vector<int> clique;
    std::vector<int> numCliqueNeighbor(N + 1, 0);

    for (int i = 0; i < N; i++) {
        int v = sorted[i];
        if (numCliqueNeighbor[v] == clique.size()) {
            clique.push_back(v);
            for (int j = 0; j < adjs[v].size(); j++) {
                numCliqueNeighbor[adjs[v][j]]++;
            }
        }
    }

    return clique;
}
