#include <vector>
#include <cassert>
#include <queue>
#include <list>
#include <iostream>

#include "CNEEObuilder.hpp"

HRG_CLIQUE::CNEEObuilder::CNEEObuilder(std::vector<std::vector<int>> adjs, int numV, int version) {
    this->numV = numV;
    this->numE = 0;
    for (int u = 0; u < numV; u++) {
        this->numE += adjs[u].size();
    }
    this->numE /= 2;

    // initialize graph container
    this->adjs = adjs;
    this->adjsC = std::vector<std::vector<int>>(numV);
    this->adjList = std::vector<std::list<int>>(numV);

    edgeQ = std::queue<std::pair<std::list<int>::iterator, std::list<int>::iterator>>();
    for (int u = 0; u < numV; u++) {
        for (int v : adjs[u]) {
            if (u > v) continue;
            adjList[u].push_back(v);
            adjList[v].push_back(u);
            edgeQ.push({prev(adjList[v].end()), prev(adjList[u].end())});
        }
    }

    // initialize cache;
    this->cache_color = std::vector<int>(numV, 0);
    this->cache_neighbor = std::vector<int>(numV, 0);
    this->V = std::vector<int>();
    this->VBits = std::vector<int>(numV, 0);

    // initialize solution container
    this->CNEEO = std::vector<std::pair<int, int>>();
    this->fails = std::vector<std::pair<int, int>>();

    // casework: version
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

void HRG_CLIQUE::CNEEObuilder::getCommonNeighbors(int u, int v)
{
    // get common neighbors
    for (int w : adjList[u]) {
        if (w == v) continue;
        VBits[w]++;
    }
    for (int w : adjList[v]) {
        if (w == u) continue;
        if (VBits[w] == 1) {
            V.push_back(w);
            VBits[w]++;
        }
    }
    for(int w : adjList[u]){
        if (w == v) continue;
        VBits[w]--;
    }
}

void HRG_CLIQUE::CNEEObuilder::clearCommonCache(int u, int v){
    for (int w : adjList[u]) {
        VBits[w] = 0;
    }
    for (int w : adjList[v]) {
        VBits[w] = 0;
    }
    V.clear();
}

bool HRG_CLIQUE::CNEEObuilder::chkCoBip() {
    long long numPartV = V.size();
    if(numPartV == 0) return true;

    // check if the # of vertices is too large
    if (numE < (numPartV*numPartV)/2) return false;

    // now |V'| = O(sqrt(|E|))

    // construct complement graph
    for (int u: V){
        for(int v: adjs[u])
            if(VBits[v]) cache_neighbor[v] = 1;
        for(int v: V){
            if(u == v) continue;
            if(cache_neighbor[v] == 0)
                adjsC[u].push_back(v);
        }
        for(int v: adjs[u])
            if(VBits[v]) cache_neighbor[v] = 0;
    }

    // check if the complement graph is bipartite
    bool is_bip = true;
    for (int u: V){
        if(cache_color[u]) continue;
        std::queue<int> q;
        q.push(u);
        cache_color[u] = 1;
        while(!q.empty()){
            int v = q.front();
            q.pop();
            for(int w: adjsC[v]){
                if(cache_color[w] == 0){
                    cache_color[w] = 3 ^ cache_color[v];
                    q.push(w);
                }
                else if(cache_color[w] == cache_color[v]){
                    is_bip = false;
                    break;
                }
            }
            if(!is_bip) break;
        }
    }

    // clear cache
    for (int u: V){
        adjsC[u].clear();
        cache_color[u] = 0;
    }

    return is_bip;
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
            failList[u].push_back({v, failList[v].end(), prev(failCont.end())});
            failList[v].push_back({u, failList[u].end(), prev(failCont.end())});
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
