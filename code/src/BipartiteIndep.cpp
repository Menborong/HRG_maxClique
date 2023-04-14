#include "BipartiteIndep.hpp"

#include <vector>

void HRG_CLIQUE::MinVertexCover::bfs(){
    queue<int> q;
    // initialize level graph
    for (int x = 0; x < NX; x++) {
        if (XtoY[x] == -1)
            level[x] = INF;
        else {
            level[x] = 0;
            q.push(x);
        }
    }
    // construct level graph
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        for (int y : adjs[x]) {
            int z = YtoX[y];
            if (z == -1 || level[z] != INF) continue;
            level[z] = level[x] + 1;
            q.push(z);
        }
    }
}

bool HRG_CLIQUE::MinVertexCover::dfs(int x){
    int adjSize = adjs[x].size();
    for (int& i = list[x]; i < adjSize; i++) {
        int y = adjs[x][i];
        int z = YtoX[y];
        if (z == -1 || (level[z] == level[x] + 1 && dfs(z))) {
            XtoY[x] = y;
            YtoX[y] = x;
            return true;
        }
    }
    return false;
}

int HRG_CLIQUE::MinVertexCover::match() {
    numMatched = 0;
    while (true) {
        bfs();
        fill(list.begin(), list.end(), 0);
        int tmp = 0;
        for (int x = 0; x < NX; x++) {
            if (XtoY[x] == -1 && dfs(x)) tmp++;
        }
        if (!tmp) break;
        numMatched += tmp;
    }
    return numMatched;
}

void HRG_CLIQUE::MinVertexCover::mvsDfs(int tar) {
    VCX[tar] = 0;
    for (int y : adjs[tar]) {
        if (YtoX[y] != -1 && !VCY[y] && VCX[YtoX[y]]) {
            VCY[y] = 1;
            mvsDfs(YtoX[y]);
        }
    }
}

void HRG_CLIQUE::MinVertexCover::minVertexCover() {
    for (int i = 0; i < NX; i++) {
        if (XtoY[i] == -1) mvsDfs(i);
    }
}


std::vector<int> HRG_CLIQUE::minVCCache1;
std::vector<int> HRG_CLIQUE::minVCCache2;

std::vector<int> HRG_CLIQUE::getBipartiteIndep(
    std::vector<int> V, std::vector<std::vector<int>> &adjs) {
    // reconstruct:
    // construct bipartite graph G = (X, Y, E)
    // X = {1, 2, ..., NX}
    // Y = {1, 2, ..., NY}

    // coloring for finding bipartite graph
    for (int v : V) {
        if ((int)minVCCache1.size() <= v) minVCCache1.resize(v + 1, 0);
        if (minVCCache1[v]) continue;
        queue<int> q;
        q.push(v);
        minVCCache1[v] = 1;
        // BFS scheme
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adjs[u]) {
                if ((int)minVCCache1.size() <= v) minVCCache1.resize(v + 1, 0);
                assert(minVCCache1[v] != minVCCache1[u]);
                if (minVCCache1[v]) continue;
                minVCCache1[v] = minVCCache1[u] ^ 3;
                q.push(v);
            }
        }
    }

    // relabeling to bipartite graph X = {0, 1, 2, ..., NX - 1}, Y = {0, 1, 2, ..., NY - 1}
    int NX = 0, NY = 0;
    for (int v : V) {
        if((int)minVCCache2.size() <= v) minVCCache2.resize(v + 1, 0);
        if (minVCCache1[v] == 1) minVCCache2[v] = NX++;
        else if (minVCCache1[v] == 2) minVCCache2[v] = NY++;
    }

    // get the minimum vertex cover
    MinVertexCover mvc(NX, NY);
    for (int u : V) {
        if (minVCCache1[u] != 1) continue;
        for (int v : adjs[u]) {
            mvc.addEdge(minVCCache2[u], minVCCache2[v]);
        }
    }

    mvc.run();

    std::vector<int> indep;
    // delete VC from V
    for (int v : V) {
        if (minVCCache1[v] == 1 && mvc.VCX[minVCCache2[v]] == 0)
            indep.push_back(v);
        if (minVCCache1[v] == 2 && mvc.VCY[minVCCache2[v]] == 0)
            indep.push_back(v);
    }

    // clear the cache
    for (int v : V) {
        minVCCache1[v] = 0;
        minVCCache2[v] = 0;
    }

    return indep;
}
