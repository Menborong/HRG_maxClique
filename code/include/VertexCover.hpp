#pragma once

#include <bits/stdc++.h>
using namespace std;
using vInt = vector<int>;
using matInt = vector<vInt>;

namespace HRG_CLIQUE {

// given a bipartite graph G = (X, Y, E), find a minimum vertex cover
// X = {1, 2, ..., N}, Y = {1, 2, ..., M}
struct MinVertexCover {
    int N, M;  // |X|, |Y|
    matInt adjs;

    // for Hopcroft-Karp algorithm
    int numMatched;
    vInt XtoY;   // XtoY[i] = j if i in X is matched to j in Y
    vInt YtoX;   // YtoX[i] = j if i in Y is matched to j in X
    vInt level;  // level[i] = distance from i to the source
    vInt list;   // list[i] = index of the next edge to be explored from i

    // for minimum vertex cover
    vInt VCX;  // VCX[i] = 1 if i in X is in the vertex cover
    vInt VCY;  // VCY[i] = 1 if i in Y is in the vertex cover

    const int INF = 1e9;

    void addEdge(int x, int y) { adjs[x].push_back(y); }

    void bfs() {
        queue<int> q;
        for (int x = 1; x <= N; x++) {
            if (XtoY[x])
                level[x] = INF;
            else {
                level[x] = 0;
                q.push(x);
            }
        }
        while (!q.empty()) {
            int x = q.front();
            q.pop();
            for (int y : adjs[x]) {
                int z = YtoX[y];
                if (!z || level[z] != INF) continue;
                level[z] = level[x] + 1;
                q.push(z);
            }
        }
    }

    bool dfs(int x) {
        int adjSize = adjs[x].size();
        for (int& i = list[x]; i < adjSize; i++) {
            int y = adjs[x][i];
            int z = YtoX[y];
            if (!z || (level[z] == level[x] + 1 && dfs(z))) {
                XtoY[x] = y;
                YtoX[y] = x;
                return true;
            }
        }
        return false;
    }

    int match() {
        numMatched = 0;
        while (true) {
            bfs();
            fill(list.begin(), list.end(), 0);
            int tmp = 0;
            for (int x = 1; x <= N; x++) {
                if (!XtoY[x] && dfs(x)) tmp++;
            }
            if (!tmp) break;
            numMatched += tmp;
        }
        return numMatched;
    }

    void mvsDfs(int tar) {
        VCX[tar] = 0;
        for (int y : adjs[tar]) {
            if (YtoX[y] && !VCY[y] && VCX[YtoX[y]]) {
                VCY[y] = 1;
                mvsDfs(YtoX[y]);
            }
        }
    }

    void minVertexCover() {
        for (int i = 1; i <= N; i++) {
            if (!XtoY[i]) mvsDfs(i);
        }
    }

    MinVertexCover(int N, int M) : N(N), M(M) {
        adjs = matInt(N + 1);
        XtoY = vInt(N + 1);
        YtoX = vInt(M + 1);
        level = vInt(N + 1);
        list = vInt(N + 1);
        VCX = vInt(N + 1, 1);
        VCY = vInt(M + 1);
        // match();
        // minVertexCover();
    }

    void run() {
        match();
        minVertexCover();
    }
};

extern std::vector<int> minVCCache1;  // for split V to X and Y
extern std::vector<int> minVCCache2;  // for relabeling vertex index

// givevn bipartite graph, find a maximum clique of the complement graph
// time complexity: O(N^{2.5}) by Hopcroft-Karp algorithm
std::vector<int> getCobipMaxClique(std::vector<int> V,
                                   std::vector<std::vector<int>>& adjs);
}  // namespace HRG_CLIQUE