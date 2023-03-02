#pragma once
#include <vector>

#include "Graph.hpp"

namespace HRG_CLIQUE {

class Reduction {
   public:

    /// @brief Apply the reduction algorithm to the graph in time $O(N+M)$
    /// @details When init size is $\Omega(n^{1-\alpha})$, then
    /// the reduced size of the graph is $O(n^{1-\alpha})$ with high probability
    /// @param adjs the adjacency list of the graph
    /// @param N the number of vertices
    /// @param initSize the initial size of the clique
    Reduction(const std::vector<std::vector<int>>& adjs, const int N,
              const int initSize);
    
    /// @brief Apply the reduction algorithm to the graph in time $O(N+M)$
    /// @details When init size is $\Omega(n^{1-\alpha})$, then
    /// the reduced size of the graph is $O(n^{1-\alpha})$ with high probability
    /// @param adjs the adjacency list of the graph
    /// @param geometry the geometry representation the graph
    /// @param N the number of vertices
    /// @param initSize the initial size of the clique
    Reduction(const std::vector<std::vector<int>>& adjs,
              const std::vector<Node>& geometry, const double R, const int N,
              const int initSize);

    std::vector<std::vector<int>>& getAdjs_red() { return adjs_red; }
    std::vector<Node>& getGeo_red() { return geo_red; }
    std::vector<int>& getFwdId() { return fwdId; }
    std::vector<int>& getBwdId() { return bwdId; }
    int getRedSize() { return redSize; }

   private:
    int redSize;
    std::vector<std::vector<int>> adjs_red;
    std::vector<Node> geo_red;
    std::vector<int> bwdId;
    std::vector<int> fwdId;
};

}  // namespace HRG_CLIQUE