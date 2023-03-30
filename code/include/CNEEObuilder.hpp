#pragma once
#include <list>
#include <queue>
#include <utility>
#include <vector>

namespace HRG_CLIQUE {
class CNEEObuilder {
   public:
    CNEEObuilder(){};  // default constructor for non initialized CNEEObuilder
    CNEEObuilder(std::vector<std::vector<int>>& adjs, int N, int version = 2);
    std::vector<std::pair<int, int>>& getCNEEO() { return CNEEO; }
    std::vector<std::pair<int, int>>& getFails() { return fails; }

   private:
    int N;
    int leftEdges;
    std::vector<std::list<int>> adjList;
    std::queue<std::pair<std::list<int>::iterator, std::list<int>::iterator>>
        edgeQ;
    std::vector<int> V;
    std::vector<int> VBits;
    // std::vector<int> cache_mark;
    std::vector<int> cache_color;
    std::vector<std::pair<int, int>> CNEEO;
    std::vector<std::pair<int, int>> fails;
    struct FailNode {
        int v;
        std::list<FailNode>::iterator failList_it;
        std::list<std::pair<std::list<int>::iterator,
                            std::list<int>::iterator>>::iterator failCont_it;
    };
    /**
     * @brief the common neighbors of u and v are stored in V.
     * VBits[v] = 1 if v is in V
     */
    void getCommonNeighbors(int u, int v);
    void clearCommonCache(int u, int v);
    /**
     * @brief check if the current graph (V) is a co-bipartite graph
     */
    bool chkCoBip();
    /**
     * @brief basic version of constructing CNEEO
     * @details time complexity: O(N^2 M^2), loop until no more edges can be
     * added
     */
    void CNEEO_ver1();

    /**
     * @brief optimized version of constructing CNEEO
     * @details time complexity: O(N^2 M^2), only edges that have potential to
     * be added are considered
     */
    void CNEEO_ver2();
};
}  // namespace HRG_CLIQUE
