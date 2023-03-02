#include "CNEEObuilder.hpp"

#include <cassert>
#include <list>
#include <queue>
#include <utility>
#include <vector>

HRG_CLIQUE::CNEEObuilder::CNEEObuilder(std::vector<std::vector<int>> &adjs,
                                       int N, int version) {
    this->N = N;
    CNEEO = std::vector<std::pair<int, int>>();
    fails = std::vector<std::pair<int, int>>();
    V = std::vector<int>();
    VBits = std::vector<int>(N + 1, 0);
    cache_mark = std::vector<int>(N + 1, 0);
    cache_color = std::vector<int>(N + 1, 0);
    adjList = std::vector<std::list<int>>(N + 1);
    for (int u = 1; u <= N; u++) {
        for (int v : adjs[u]) {
            if (u > v) continue;
            adjList[u].push_back(v);
            adjList[v].push_back(u);
            edgeQ.push({prev(adjList[v].end()), prev(adjList[u].end())});
        }
    }

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
    // only use vertices in V
    // and VBits should be 1

    // initialize cache;
    for (int u : V) {
        cache_mark[u] = 0;
        cache_color[u] = 0;
    }

    // BFS
    for (int u : V) {
        if (cache_color[u]) continue;
        cache_color[u] = 1;
        std::queue<int> q;
        q.push(u);
        while (!q.empty()) {
            int tar = q.front();
            q.pop();
            std::vector<int> nxts;
            // mark non-next vertices
            for (int nonnxt : adjList[tar]) {
                if (!VBits[nonnxt]) continue;
                cache_mark[nonnxt] = 1;
            }
            // find next vertices
            for (int v : V) {
                if (v == tar) continue;
                if (!cache_mark[v]) nxts.push_back(v);
            }
            // clear cache
            for (int nonnxt : adjList[tar]) {
                if (!VBits[nonnxt]) continue;
                cache_mark[nonnxt] = 0;
            }
            // check bipartiteness
            for (int nxt : nxts) {
                if (cache_color[nxt] == cache_color[tar]) return false;
                if (cache_color[nxt] && cache_color[nxt] != cache_color[tar])
                    continue;
                cache_color[nxt] = 3 - cache_color[tar];
                q.push(nxt);
            }
        }
    }

    return true;
}

void HRG_CLIQUE::CNEEObuilder::CNEEO_ver1() {
    std::queue<std::pair<std::list<int>::iterator, std::list<int>::iterator>>
        failCont;

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
    std::vector<std::list<FailNode>> failList(N + 1);

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
