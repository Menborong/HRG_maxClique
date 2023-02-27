#pragma once
#include <vector>

#include "Graph.hpp"

namespace HRG_CLIQUE {

class Reduction {
   public:
    // get the new vertex set after reduction
    // by deleting all vertices with degree lower than initSize
    // until there is no such a vertex.
    // time complexity: O(N+M)
    Reduction(const std::vector<std::vector<int>>& adjs, const int N,
              const int initSize);
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