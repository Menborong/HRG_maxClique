#pragma once
#include <vector>
#include <utility>
#include <list>

namespace HRG_CLIQUE{

    bool chkCoBip(std::vector<std::list<int>> &adjs, std::vector<int> &V, std::vector<int> &VBits,
                  std::vector<int> &cache, std::vector<int> &colors);
    // get CNEEO: edge order set
    // time complexity: O(N^2 M^2)
    std::vector<std::pair<int, int>> getCNEEO(std::vector<std::vector<int>>& adjs, int N);
}
