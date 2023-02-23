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


    class CNEEObuilder{
    public:
        CNEEObuilder(std::vector<std::vector<int>>& adjs, int N, int version = 2);
        std::vector<std::pair<int, int>> getCNEEO();
    private:
        std::vector<std::list<int>> adjs;
        std::vector<int> V;
        std::vector<int> VBits;
        std::vector<int> cache_mark;
        std::vector<int> cache_color;
        std::vector<std::pair<int, int>> CNEEO;
        bool chkCoBip();

        /**
         * @brief CNEEO_ver1: basic version of constructing CNEEO
         * @details time complexity: O(N^2 M^2), loop until no more edges can be added
         */
        void CNEEO_ver1();

        /**
         * @brief CNEEO_ver2: optimized version of constructing CNEEO
         * @details time complexity: O(N^2 M^2), only edges that have potential to be added are considered
         */
        void CNEEO_ver2();
    };
}
