#pragma once

#include <vector>

namespace HRG_CLIQUE {

class Reduction {
   public:
    // get the new vertex set after reduction
    // by deleting all vertices with degree lower than redSize
    // until there is no such a vertex.
    // time complexity: O(N+M)
    void reduce(const std::vector<std::vector<int>>& adjs,
                                    int N, int redSize);
    void reduce(const std::vector<std::vector<int>>& adjs, const std::vector<Node> &geometry, double R,
                                    int N, int redSize);

    std::vector<int>& getV_red() { return V_red; }
    std::vector<std::vector<int>>& getAdjs_red() { return adjs_red; }
    private:
    std::vector<int> V_red;
    std::vector<std::vector<int>> adjs_red;
    std::vector<int> 
};

// get the new vertex set after reduction
// by deleting all vertices with degree lower than redSize
// until there is no such a vertex.
// time complexity: O(N+M)
std::vector<int> reduction(const std::vector<std::vector<int>>& adjs, int N,
                           int redSize);

}  // namespace HRG_CLIQUE