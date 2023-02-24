#include "reduction.hpp"

#include <queue>
#include <vector>

// using namespace std;
// using vInt = vector<int>;
// using matInt = vector<vInt>;

namespace HRG_CLIQUE {

std::vector<int> reduction(const std::vector<std::vector<int>>& adjs, int N,
                           int redSize) {
    // BFS scheme
    std::queue<int> q;
    std::vector<int> degree(N + 1);
    for (int i = 1; i <= N; i++) {
        degree[i] = adjs[i].size();
        if (degree[i] < redSize) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int tar = q.front();
        q.pop();
        for (int nxt : adjs[tar]) {
            if (degree[nxt] < redSize) {  // already deleted
                continue;
            }
            degree[nxt]--;
            if (degree[nxt] < redSize) {
                q.push(nxt);
            }
        }
    }

    std::vector<int> remains;
    for (int i = 1; i <= N; i++) {
        if (degree[i] >= redSize) {
            remains.push_back(i);
        }
    }

    return remains;
}
}  // namespace HRG_CLIQUE