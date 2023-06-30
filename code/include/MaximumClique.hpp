#pragma once
#include <vector>

#include "BipartiteIndep.hpp"
#include "Graph.hpp"

namespace HRG_CLIQUE {

/**
 * @brief get the maximum clique of hyperbolic random graph.
 * @details The algorithm is based on the paper.
 * If the geometry is given, the time complexiy is $O(n^{4.5(1-\alpha)})$ whp.
 * Otherwise, the time complexity is $O(n^{6(1-\alpha)})$ whp.
 * If the maximum clique is not given, the algorithm gives the lower bound of
 * the maximum clique, and reduce the graph to a smaller size.
 */
class MaxClique {
   public:
    /**
     * @brief Construct a new Max Clique object
     *
     * @param adjs the adjacency list of the graph
     * @param N the number of vertices
     * @param version
     * version = 0: default (maxCliqueOpt);
     * version = 1: maxClique;
     * version = 2: maxCliqueRed;
     * version = 3: maxCliqueSkip;
     * version = 4: maxCliqueOpt
     * @param geometry
     * If geometry = {}, it runs to maxCliqueNoGeo
     * @param R
     */
    MaxClique(const std::vector<std::vector<int>>& adjs, const int N,
              const int version = 0, const std::vector<Node>& geometry = {},
              const double R = 0.0)
        : adjs(adjs),
          N(N),
          version(version),
          geometry(geometry),
          R(R),
          hasGeo(geometry.size() != 0) {}
    void run();
    std::vector<int> getMaxClique() const { return maxClique; }
    std::vector<std::pair<int, int>> getFailedEdges() const {
        return failedEdges;
    }

   private:
    const std::vector<std::vector<int>>& adjs;
    const int N;
    const int version;
    const std::vector<Node>& geometry;
    const double R;
    const bool hasGeo;

    std::vector<int> maxClique;
    std::vector<std::pair<int, int>> failedEdges;

    void maxCliqueNoGeoV1();
    void maxCliqueNoGeoV2();
    void maxCliqueGeoV1();
    void maxCliqueGeoV2();
    void maxCliqueGeoV3();
    void maxCliqueGeoV4();
};
}  // namespace HRG_CLIQUE