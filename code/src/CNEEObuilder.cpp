#include "CNEEObuilder.hpp"

#include <cassert>
#include <list>
#include <queue>
#include <utility>
#include <vector>

#include <iostream>

HRG_CLIQUE::CNEEObuilder::CNEEObuilder(std::vector<std::vector<int>> &adjs,
                                       int numV, int version) {
    this->numV = numV;
    this->numE = 0;
    for (int u = 0; u < numV; u++) {
        this->numE += adjs[u].size();
    }
    this->numE /= 2;
    
    // initialize graph container
    this->adjs = adjs;
    adjsC = std::vector<std::vector<int>>(numV);
    adjList = std::vector<std::list<int>>(numV);
    for (int u = 0; u < numV; u++) {
        for (int v : adjs[u]) {
            if (u > v) continue;
            adjList[u].push_back(v);
            adjList[v].push_back(u);
            edgeQ.push({prev(adjList[v].end()), prev(adjList[u].end())});
        }
    }

    // initialize cache;
    cache_color = std::vector<int>(numV, 0);
    cache_neighbor = std::vector<int>(numV, 0);
    V = std::vector<int>();
    VBits = std::vector<int>(numV, 0);

    // initialize solution container
    CNEEO = std::vector<std::pair<int, int>>();
    fails = std::vector<std::pair<int, int>>();
    

    switch (version) {
        case 1:
            CNEEO_ver1();
            break;
        case 2:
            CNEEO_ver2();
            break;
        default:
            assert(false);  // invalid version
    }
}

void HRG_CLIQUE::CNEEObuilder::getCommonNeighbors(int u, int v) {
    // find common neighbors
    V.clear();
    for (int tar : adjList[u]) {
        if (tar == v) continue;
        VBits[tar]++;
    }
    for (int tar : adjList[v]) {
        if (tar == u) continue;
        if (VBits[tar] == 1) VBits[tar]++;
    }
    for (int tar : adjList[u]) {
        if (tar == v) continue;
        if (VBits[tar] == 2) V.push_back(tar);
        VBits[tar]--;
    }
}

void HRG_CLIQUE::CNEEObuilder::clearCommonCache(int u, int v) {
    // clear cache
    for (int tar : adjList[u]) {
        if (tar == v) continue;
        VBits[tar] = 0;
    }
    for (int tar : adjList[v]) {
        if (tar == u) continue;
        VBits[tar] = 0;
    }
}

bool HRG_CLIQUE::CNEEObuilder::chkCoBip() {
    long long numVp = V.size();
    if(numVp == 0) return true;
    // check if the number of vertices is too large
    // now |V'| = O(sqrt(|E|)
    if (numE < (numVp - 1) * (numVp - 1) / 2) return false;

    // constructing complement graph (O(E + V'^2) = O(E))
    for(int u: V){
        for(int v: adjs[u])
            if(VBits[v]) cache_neighbor[v]++;
        for(int v: V){
            if(u == v) continue;
            if(cache_neighbor[v] == 0) adjsC[u].push_back(v);
        }
        for(int v: adjs[u])
            if(VBits[v]) cache_neighbor[v]--;
    }

    // check if the complement graph is bipartite
    bool is_bipartite = true;
    for(int u: V){
        if(cache_color[u]) continue;
        cache_color[u] = 1;
        std::queue<int> q;
        q.push(u);
        while(!q.empty()){
            int tar = q.front();
            q.pop();
            for(int nxt: adjsC[tar]){
                if(cache_color[nxt] == cache_color[tar]){
                    is_bipartite = false;
                    break;
                }
                if(cache_color[nxt] && cache_color[nxt] != cache_color[tar]) continue;
                cache_color[nxt] = 3 - cache_color[tar];
                q.push(nxt);
            }
            if(!is_bipartite) break;
        }
        if(!is_bipartite) break;
    }

    // clear cache
    for(int u: V){
        cache_color[u] = 0;
        adjsC[u].clear();
    }

    return is_bipartite;
}

void HRG_CLIQUE::CNEEObuilder::CNEEO_ver1() {
    std::queue<std::pair<std::list<int>::iterator, std::list<int>::iterator>>
        failCont;
        
    int lastChk = 0; // for debug
    int lastCount = 0;

    while (!edgeQ.empty()) {
        auto it1 = edgeQ.front().first;
        auto it2 = edgeQ.front().second;
        edgeQ.pop();
        int u = *it1;
        int v = *it2;

        // find common neighbors
        getCommonNeighbors(u, v);
        bool result = chkCoBip();
        clearCommonCache(u, v);

        if (result) {
            CNEEO.push_back({u, v});
            adjList[v].erase(it1);
            adjList[u].erase(it2);
            while (!failCont.empty()) {
                edgeQ.push(failCont.front());
                failCont.pop();
            }

            lastCount++;
            if(lastChk < lastCount){
                lastChk += numE/100;
                std::cout << "\rprogress: " << lastCount << "/" << numE;
                std::cout.flush();
            }
            
        } else {
            failCont.push({it1, it2});
        }
    }

    // fill the rest of edges to fails;
    while (!failCont.empty()) {
        fails.push_back({*failCont.front().first, *failCont.front().second});
        failCont.pop();
    }
}

void HRG_CLIQUE::CNEEObuilder::CNEEO_ver2() {
    std::list<std::pair<std::list<int>::iterator, std::list<int>::iterator>>
        failCont;
    std::vector<std::list<FailNode>> failList(numV);

    int lastChk = 0; // for debug
    int lastCount = 0;

    while (!edgeQ.empty()) {
        auto it1 = edgeQ.front().first;
        auto it2 = edgeQ.front().second;
        edgeQ.pop();
        int u = *it1;
        int v = *it2;

        // find common neighbors
        getCommonNeighbors(u, v);
        bool result = chkCoBip();
        clearCommonCache(u, v);

        if (result) {
            CNEEO.push_back({u, v});
            adjList[v].erase(it1);
            adjList[u].erase(it2);

            for (auto it = failList[u].begin(); it != failList[u].end(); it++) {
                edgeQ.push(*it->failCont_it);
                failCont.erase(it->failCont_it);
                failList[it->v].erase(it->failList_it);
            }
            for (auto it = failList[v].begin(); it != failList[v].end(); it++) {
                edgeQ.push(*it->failCont_it);
                failCont.erase(it->failCont_it);
                failList[it->v].erase(it->failList_it);
            }
            failList[u].clear();
            failList[v].clear();

            lastCount++;
            if(lastChk < lastCount){
                lastChk += numE/100;
                std::cout << "\rprogress: " << lastCount << "/" << numE;
                std::cout.flush();
            }
        } else {
            failCont.push_back({it1, it2});
            failList[u].push_back({v, failList[u].end(), prev(failCont.end())});
            failList[v].push_back({u, failList[v].end(), prev(failCont.end())});
            failList[u].back().failList_it = prev(failList[v].end());
            failList[v].back().failList_it = prev(failList[u].end());
        }
    }

    // fill the rest of edges to fails;
    while (!failCont.empty()) {
        fails.push_back({*failCont.front().first, *failCont.front().second});
        failCont.pop_front();
    }
}
