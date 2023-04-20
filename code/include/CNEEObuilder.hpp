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
    // int N;
    // int leftEdges;
    int numV;
    int numE;
    std::vector<std::vector<int>> adjs; // input graph
    std::vector<std::vector<int>> adjsC; // complement graph of induced subgraph of common neighbors
    std::vector<std::list<int>> adjList; // subgraph of remaining edges

    // cache list
    std::vector<int> cache_color; // cache for 2-coloring
    std::vector<int> cache_neighbor; // cache for finding complement graph
    std::vector<int> V; // common neighbors
    std::vector<int> VBits; // common neighbors bitset

    std::vector<std::pair<int, int>> CNEEO; // solution
    std::vector<std::pair<int, int>> fails; // failed edges (if input graph does not accept CNEEO)

    std::queue<std::pair<std::list<int>::iterator, std::list<int>::iterator>>
        edgeQ; // queue of edges that have potential to be added

    // only use for version 2
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
     * @brief check if the current induced subgraph is a co-bipartite graph
     * @details time complexity: O(N+M)
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
