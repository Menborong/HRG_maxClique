#include "vertexCover.hpp"
#include <vector>

std::vector<int> HRG_CLIQUE::minVCCache1;
std::vector<int> HRG_CLIQUE::minVCCache2;

std::vector<int> HRG_CLIQUE::getCobipMaxClique(std::vector<int> V, std::vector<std::vector<int>> &adjs)
{
    // reconstruct:
    // construct bipartite graph G = (X, Y, E)
    // X = {1, 2, ..., NX}
    // Y = {1, 2, ..., NY}

    // coloring for finding bipartite graph
    for(int v: V){
        if(minVCCache1.size() <= v) minVCCache1.resize(v+1, 0);
        if(minVCCache1[v]) continue;
        queue<int> q;
        q.push(v);
        minVCCache1[v] = 1;
        // BFS scheme
        while(!q.empty()){
            int u = q.front();
            q.pop();
            for(int v: adjs[u]){
                if(minVCCache1.size() <= v) minVCCache1.resize(v+1, 0);
                assert(minVCCache1[v] != minVCCache1[u]);
                if(minVCCache1[v]) continue;
                minVCCache1[v] = minVCCache1[u]^3;
                q.push(v);
            }
        }
    }

    // relabeling
    int xNow = 0, yNow = 0;
    for(int v: V){
        if(minVCCache2.size() <= v) minVCCache2.resize(v+1, 0);
        if(minVCCache1[v] == 1) minVCCache2[v] = ++xNow;
        else if(minVCCache1[v] == 2) minVCCache2[v] = ++yNow;

    }

    // get the minimum vertex cover
    MinVertexCover mvc(xNow, yNow);
    for(int u: V){
        if(minVCCache1[u] != 1) continue;
        for(int v: adjs[u]){
            mvc.addEdge(minVCCache2[u], minVCCache2[v]);
        }
    }

    mvc.run();

    std::vector<int> clique;
    // delete VC from V
    for(int v: V){
        if(minVCCache1[v] == 1 && mvc.VCX[minVCCache2[v]] == 0) clique.push_back(v);
        if(minVCCache1[v] == 2 && mvc.VCY[minVCCache2[v]] == 0) clique.push_back(v);
    }

    // clear the cache
    for(int v: V){
        minVCCache1[v] = 0;
        minVCCache2[v] = 0;
    }

    return clique;
}
