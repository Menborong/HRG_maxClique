#include "CNEEObuilder.hpp"
#include <vector>
#include <list>
#include <queue>
#include <utility>

bool HRG_CLIQUE::chkCoBip(std::vector<std::list<int>> &adjs, std::vector<int> &V, std::vector<int> &VBits,
                          std::vector<int> &cache, std::vector<int> &colors)
{
    // initialize cache;
    for(int u: V){
        cache[u] = 0;
        colors[u] = 0;
    }

    std::queue<int> targetVs;

    for(int u: V){
        if(colors[u]) continue;
        colors[u] = 1;
        std::queue<int> q;
        q.push(u);
        targetVs.push(u);
        while(!q.empty()){
            int tar = q.front();
            q.pop();
            std::vector<int> nxts;
            for(int nonnxt: adjs[tar]){
                if(VBits[nonnxt] != 2) continue;
                cache[nonnxt] = 1;
            }
            for(int v: V){
                if(v == tar) continue;
                if(!cache[v]) nxts.push_back(v);
            }
            // clear cache
            for(int nonnxt: adjs[tar]){
                if(VBits[nonnxt] != 2) continue;
                cache[nonnxt] = 0;
            }

            for(int nxt: nxts){
                if(colors[nxt] == colors[tar]) return false;
                if(colors[nxt] && colors[nxt] != colors[tar]) continue;
                colors[nxt] = 3 - colors[tar];
                q.push(nxt);
                targetVs.push(u);
            }
        }
    }

    // clear colors
    while(!targetVs.empty()){
        int u = targetVs.front();
        targetVs.pop();
        colors[u] = 0;
    }

    return true;
}

std::vector<std::pair<int, int>> HRG_CLIQUE::getCNEEO(std::vector<std::vector<int>> &adjs, int N)
{
    std::vector<std::pair<int, int>> CNEEO;

    std::vector<std::list<int>> adjList(N+1); // for the edge deletion in O(1)
    std::queue<std::pair<std::list<int>::iterator, std::list<int>::iterator>> q;
    for(int u=1; u<=N; u++){
        for(int v: adjs[u]){
            if(u > v) continue;
            adjList[u].push_back(v);
            adjList[v].push_back(u);
            q.push({prev(adjList[v].end()), prev(adjList[u].end())});
        }
    }

    std::vector<int> cache1(N+1); // for finding common neighbors
    std::vector<int> cache2(N+1); // for finding complement neighbors
    std::vector<int> colorCache(N+1); // for checking bipartiteness

    int now = 0;
    std::vector<std::pair<int, int>> fails;

    while(!q.empty()){
        auto it1 = q.front().first;
        auto it2 = q.front().second;
        q.pop();
        int u = *it1;
        int v = *it2;
        std::vector<int> commons;
        for(int tar: adjList[u]){
            if(tar == v) continue;
            cache1[tar]++;
        }
        for(int tar: adjList[v]){
            if(tar == u) continue;
            cache1[tar]++;
            if(cache1[tar] == 2)
                commons.push_back(tar);
        }
        
        bool result = chkCoBip(adjList, commons, cache1, cache2, colorCache);

        // clear cache
        for(int tar: adjList[u])
            cache1[tar] = 0;
        for(int tar: adjList[v])
            cache1[tar] = 0;

        if(result){
            CNEEO.push_back({u, v});
            adjList[v].erase(it1); 
            adjList[u].erase(it2);
            now = 0;
        }
        else{
            now++;
            if(now >= q.size()+1){
                fails.push_back({u, v});
                continue;
            }
            q.push({it1, it2});


        }
    }

    return CNEEO;
}

HRG_CLIQUE::CNEEObuilder::CNEEObuilder(std::vector<std::vector<int>> &adjs, int N, int version)
{
    VBits = std::vector<int>(N+1, 0);
    cache_mark = std::vector<int>(N+1, 0);
    cache_color = std::vector<int>(N+1, 0);
    
}

bool HRG_CLIQUE::CNEEObuilder::chkCoBip()
{
    // only use vertices in V
    // and VBits should be 1

    // initialize cache;
    for(int u: V){
        cache_mark[u] = 0;
        cache_color[u] = 0;
    }

    // BFS
    for(int u: V){
        if(cache_color[u]) continue;
        cache_color[u] = 1;
        std::queue<int> q;
        q.push(u);
        while(!q.empty()){
            int tar = q.front();
            q.pop();
            std::vector<int> nxts;
            // mark non-next vertices
            for(int nonnxt: adjs[tar]){
                if(!VBits[nonnxt]) continue;
                cache_mark[nonnxt] = 1;
            }
            // find next vertices
            for(int v: V){
                if(v == tar) continue;
                if(!cache_mark[v]) nxts.push_back(v);
            }
            // clear cache
            for(int nonnxt: adjs[tar]){
                if(!VBits[nonnxt]) continue;
                cache_mark[nonnxt] = 0;
            }
            // check bipartiteness
            for(int nxt: nxts){
                if(cache_color[nxt] == cache_color[tar]) return false;
                if(cache_color[nxt] && cache_color[nxt] != cache_color[tar]) continue;
                cache_color[nxt] = 3 - cache_color[tar];
                q.push(nxt);
            }
        }
    }

    return true;
}
