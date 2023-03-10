#include "Reduction.hpp"

#include <queue>
#include <vector>

#include "Graph.hpp"

// using namespace std;
// using vInt = vector<int>;
// using matInt = vector<vInt>;

namespace HRG_CLIQUE {

Reduction::Reduction(const std::vector<std::vector<int>>& adjs, const int N,
                     const int initSize) {
    // BFS scheme
    std::queue<int> q;
    std::vector<int> degree(N + 1);
    for (int i = 1; i <= N; i++) {
        degree[i] = adjs[i].size();
        if (degree[i] < initSize) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int tar = q.front();
        q.pop();
        for (int nxt : adjs[tar]) {
            if (degree[nxt] < initSize) {  // already deleted
                continue;
            }
            degree[nxt]--;
            if (degree[nxt] < initSize) {
                q.push(nxt);
            }
        }
    }

    redSize = 0;
    fwdId = std::vector<int>(N + 1, -1);
    bwdId = std::vector<int>(1, 0);
    for (int i = 1; i <= N; i++) {
        if (degree[i] >= redSize) {
            fwdId[i] = ++redSize;
            bwdId.push_back(i);
        }
    }

    // reconstruct adjs list
    adjs_red = std::vector<std::vector<int>>(redSize + 1);
    for (int i = 1; i <= N; i++) {
        if (fwdId[i] == -1) continue;
        for (int j : adjs[i]) {
            if (fwdId[j] == -1) continue;
            adjs_red[fwdId[i]].push_back(fwdId[j]);
        }
    }
}

Reduction::Reduction(const std::vector<std::vector<int>>& adjs,
                     const std::vector<Node>& geometry,
                     const int N, const int initSize) {
    Reduction(adjs, N, initSize);
    geo_red = std::vector<Node>(redSize + 1);
    for (int i = 1; i <= redSize; i++) geo_red[i] = geometry[bwdId[i]];
}

}  // namespace HRG_CLIQUE