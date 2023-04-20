#pragma once

#include <vector>

using namespace std;
using vInt = vector<int>;
using matInt = vector<vInt>;

namespace HRG_CLIQUE {

// given a bipartite graph G = (X, Y, E), find a minimum vertex cover
// X = {0, 2, ..., NX-1}, Y = {1, 2, ..., NY-1}
struct MinVertexCover {
    int NX, NY;  // |X|, |Y|
    std::vector<std::vector<int>> adjs;

    // for Hopcroft-Karp algorithm
    int numMatched;
    std::vector<int> XtoY;   // XtoY[i] = j if i in X is matched to j in Y
    std::vector<int> YtoX;   // YtoX[i] = j if i in Y is matched to j in X
    std::vector<int> level;  // level[i] = distance from i to the source
    std::vector<int> list;   // list[i] = index of the next edge to be explored from i

    // for minimum vertex cover
    std::vector<int> VCX;  // VCX[i] = 1 if i in X is in the vertex cover
    std::vector<int> VCY;  // VCY[i] = 1 if i in Y is in the vertex cover

    const int INF = 1e9;

    void addEdge(int x, int y) { adjs[x].push_back(y); }

    void bfs();
    bool dfs(int x);
    int match();
    void mvsDfs(int tar);
    void minVertexCover();

    MinVertexCover(int NX, int NY) : NX(NX), NY(NY) {
        adjs = matInt(NX);
        XtoY = vInt(NX, -1);
        YtoX = vInt(NY, -1);
        level = vInt(NX);
        list = vInt(NX);
        VCX = vInt(NX, 1);
        VCY = vInt(NY);
    }

    void run() {
        match();
        minVertexCover();
    }
};

extern std::vector<int> minVCCache1;  // for split V to X and Y
extern std::vector<int> minVCCache2;  // for relabeling vertex index

// givevn bipartite graph, find a maximum independent set
// time complexity: O(N^{2.5}) by Hopcroft-Karp algorithm
std::vector<int> getBipartiteIndep(std::vector<int> V,
                                   std::vector<std::vector<int>>& adjs);
}  // namespace HRG_CLIQUE