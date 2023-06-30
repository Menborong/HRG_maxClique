#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>

#include "MaximumClique.hpp"
#include "CNEEObuilder.hpp"
#include "Graph.hpp"
#include "MaximalClique.hpp"
#include "Reduction.hpp"
#include "BipartiteIndep.hpp"


void HRG_CLIQUE::MaxClique::run() {
    // initalize solution
    maxClique = vector<int>();
    failedEdges = vector<std::pair<int, int>>();

    // with geometry
    if (hasGeo) {
        switch (version) {
            case 0:
                // optimized version (= ver.4)
                maxCliqueGeoV4();
                break;
            case 1:
                // original version
                maxCliqueGeoV1();
                break;
            case 2:
                // reduction version
                maxCliqueGeoV2();
                break;
            case 3:
                // reduction + skip low degree vertices
                maxCliqueGeoV3();
                break;
            case 4:
                // reduction + skip low degree vertices + skip + edge ordering
                maxCliqueGeoV4();
                break;

            default:
                assert(false);
        }
    }
    // without geometry
    else {
        switch (version) {
            case 0:
                // optimized version (= ver. 2)
                maxCliqueNoGeoV2();
                break;
            case 1:
                // original version (CNEEO w/ reduction)
                maxCliqueNoGeoV1();
                break;
            case 2:
                // optimized CNEEO construction vertion
                maxCliqueNoGeoV2();
                break;
            default:
                assert(false);
        }
    }
}

void HRG_CLIQUE::MaxClique::maxCliqueNoGeoV1() {
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    maxClique = init_solution;

    // reduction
    Reduction red(adjs, N, maxClique.size());

    std::vector<std::vector<int>> adjs_red = red.getAdjs_red();
    std::vector<int> bwdId = red.getBwdId();
    int redSize = red.getRedSize();

    // construct CNEEO
    CNEEObuilder CNEEObuild(adjs_red, redSize, 1);
    std::vector<std::pair<int, int>> CNEEO = CNEEObuild.getCNEEO();
    
    // reconstruct the graph
    // add the edges in reverse order of CNEEO
    std::vector<vector<int>> newAdjs(redSize);

    // this adjs matrix is reuse for the efficiency
    std::vector<std::vector<int>> cobipartite(redSize);
    std::vector<int> commonCache(redSize, 0);
    std::vector<int> neighborCache(redSize, 0);

    // add all failed edges to new adjacnet matrix
    failedEdges = CNEEObuild.getFails();
    int failSize = failedEdges.size();
    for(int i=0; i<failSize; i++){
        int u = failedEdges[i].first;
        int v = failedEdges[i].second;
        newAdjs[u].push_back(v);
        newAdjs[v].push_back(u);
    }

    // main iteration
    for (int i = CNEEO.size() - 1; i >= 0; i--) {
        int u = CNEEO[i].first;
        int v = CNEEO[i].second;
        newAdjs[u].push_back(v);
        newAdjs[v].push_back(u);
        if (newAdjs[u].size() < maxClique.size()) continue;
        if (newAdjs[v].size() < maxClique.size()) continue;

        // collect all common neighbor vertices
        std::vector<int> vertices;
        for (int k : newAdjs[u]) {
            if (k == v) continue;
            if (newAdjs[k].size() < maxClique.size()) continue;
            commonCache[k]++;
        }
        for (int k : newAdjs[v]) {
            if (k == u) continue;
            if (newAdjs[k].size() < maxClique.size()) continue;
            commonCache[k]++;
            if (commonCache[k] == 2) {
                vertices.push_back(k);
            }
        }

        // |vertices|+2 <= |solution|: can not be a clique, skip
        if (vertices.size() + 2 <= maxClique.size()) {
            // clear commonCache
            for (int k : newAdjs[u]) {
                if (k == v) continue;
                commonCache[k] = 0;
            }
            for (int k : newAdjs[v]) {
                if (k == u) continue;
                commonCache[k] = 0;
            }
            continue;
        }

        // need constructing complement graph of induced subgraph of G
        // time complexity: O(|V|^2)
        for (int a : vertices) {
            // check neighbors in a graph
            for (int nei : adjs_red[a]) {
                if (nei == u || nei == v) continue;
                if (commonCache[nei] == 2) {
                    neighborCache[nei]++;
                }
            }

            // find complement edges
            for (int b : vertices) {
                if (a == b) continue;
                if (!neighborCache[b]) {
                    cobipartite[a].push_back(b);
                }
            }

            // clear neighborCache
            for (int nei : adjs_red[a]) {
                if (commonCache[nei] == 2) {
                    neighborCache[nei] = 0;
                }
            }
        }

        // clear commonCache
        for (int k : newAdjs[u]) {
            commonCache[k] = 0;
        }
        for (int k : newAdjs[v]) {
            commonCache[k] = 0;
        }

        // get candidate clique
        std::vector<int> candidate = getBipartiteIndep(vertices, cobipartite);
        candidate.push_back(u);
        candidate.push_back(v);

        // update solution
        if (candidate.size() > maxClique.size()) {
            maxClique = candidate;
        }

        // clear cobipartite graph
        for (int a : vertices) {
            cobipartite[a].clear();
        }
    }

    // restore the vertex number
    if (maxClique.size() > init_solution.size()) {
        for (int& v : maxClique) {
            v = bwdId[v];
        }
    }

    // get failed edges & restore the vertex number
    for (auto& e : failedEdges) {
        e.first = bwdId[e.first];
        e.second = bwdId[e.second];
    }
}

void HRG_CLIQUE::MaxClique::maxCliqueNoGeoV2() {
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    maxClique = init_solution;

    // reduction
    Reduction red(adjs, N, maxClique.size());

    std::vector<std::vector<int>> adjs_red = red.getAdjs_red();
    std::vector<int> bwdId = red.getBwdId();
    int redSize = red.getRedSize();

    // construct CNEEO
    CNEEObuilder CNEEObuild(adjs_red, redSize, 2);
    std::vector<std::pair<int, int>> CNEEO = CNEEObuild.getCNEEO();

    // reconstruct the graph
    // add the edges in reverse order of CNEEO
    std::vector<vector<int>> newAdjs(redSize);

    // this adjs matrix is reuse for the efficiency
    std::vector<std::vector<int>> cobipartite(redSize);
    std::vector<int> commonCache(redSize, 0);
    std::vector<int> neighborCache(redSize, 0);

    // add all failed edges to new adjacnet matrix
    failedEdges = CNEEObuild.getFails();
    int failSize = failedEdges.size();
    for(int i=0; i<failSize; i++){
        int u = failedEdges[i].first;
        int v = failedEdges[i].second;
        newAdjs[u].push_back(v);
        newAdjs[v].push_back(u);
    }

    // main iteration
    for (int i = CNEEO.size() - 1; i >= 0; i--) {
        int u = CNEEO[i].first;
        int v = CNEEO[i].second;
        newAdjs[u].push_back(v);
        newAdjs[v].push_back(u);
        if (newAdjs[u].size() < maxClique.size()) continue;
        if (newAdjs[v].size() < maxClique.size()) continue;

        // collect all common neighbor vertices
        std::vector<int> vertices;
        for (int k : newAdjs[u]) {
            if (k == v) continue;
            if (newAdjs[k].size() < maxClique.size()) continue;
            commonCache[k]++;
        }
        for (int k : newAdjs[v]) {
            if (k == u) continue;
            if (newAdjs[k].size() < maxClique.size()) continue;
            commonCache[k]++;
            if (commonCache[k] == 2) {
                vertices.push_back(k);
            }
        }

        // |vertices|+2 <= |solution|: can not be a clique, skip
        if (vertices.size() + 2 <= maxClique.size()) {
            // clear commonCache
            for (int k : newAdjs[u]) {
                if (k == v) continue;
                commonCache[k] = 0;
            }
            for (int k : newAdjs[v]) {
                if (k == u) continue;
                commonCache[k] = 0;
            }
            continue;
        }

        // need constructing complement graph of induced subgraph
        // time complexity: O(|V|^2)
        for (int a : vertices) {
            // check neighbors in a graph
            for (int nei : adjs_red[a]) {
                if (nei == u || nei == v) continue;
                if (commonCache[nei] == 2) {
                    neighborCache[nei]++;
                }
            }

            // find complement edges
            for (int b : vertices) {
                if (a == b) continue;
                if (!neighborCache[b]) {
                    cobipartite[a].push_back(b);
                }
            }

            // clear neighborCache
            for (int nei : adjs_red[a]) {
                if (commonCache[nei] == 2) {
                    neighborCache[nei] = 0;
                }
            }
        }

        // clear commonCache
        for (int k : newAdjs[u]) {
            commonCache[k] = 0;
        }
        for (int k : newAdjs[v]) {
            commonCache[k] = 0;
        }

        // get candidate clique
        std::vector<int> candidate = getBipartiteIndep(vertices, cobipartite);
        candidate.push_back(u);
        candidate.push_back(v);

        // update solution
        if (candidate.size() > maxClique.size()) {
            maxClique = candidate;
        }

        // clear cobipartite graph
        for (int a : vertices) {
            cobipartite[a].clear();
        }
    }

    // restore the vertex number
    if (maxClique.size() > init_solution.size()) {
        for (int& v : maxClique) {
            v = bwdId[v];
        }
    }

    // get failed edges & restore the vertex number
    failedEdges = CNEEObuild.getFails();
    for (auto& e : failedEdges) {
        e.first = bwdId[e.first];
        e.second = bwdId[e.second];
    }
}

void HRG_CLIQUE::MaxClique::maxCliqueGeoV1() {
    // this adjs matrix is reused for the efficiency
    std::vector<std::vector<int>> cobipartite(N);

    // main iteration
    for (int u = 0; u < N; u++) {
        for (int v : adjs[u]) {
            double d = getDist(geometry[u], geometry[v]);
            // construct co-bipartite graph
            std::vector<int> vertices;

            // find common neighbor vertices
            // for speed up, choose the smaller one
            int tar = (adjs[u].size() < adjs[v].size()) ? u : v;
            for (int k : adjs[tar]) {
                if (k == u || k == v) continue;
                if (adjs[k].size() < maxClique.size()) continue;
                if (getDist(geometry[u], geometry[k]) > d ||
                    getDist(geometry[v], geometry[k]) > d)
                    continue;
                vertices.push_back(k);
            }

            // construct graph
            for (int a : vertices) {
                for (int b : vertices) {
                    if (a == b) continue;
                    if (getDist(geometry[a], geometry[b]) > d) {
                        cobipartite[a].push_back(b);
                    }
                }
            }

            // get clique
            std::vector<int> candidate =
                getBipartiteIndep(vertices, cobipartite);
            candidate.push_back(u);
            candidate.push_back(v);
            

            // update solution
            if (candidate.size() > maxClique.size()) {
                maxClique = candidate;
            }

            // clear cobipartite graph
            for (int a : vertices) {
                cobipartite[a].clear();
            }
        }
    }
}

void HRG_CLIQUE::MaxClique::maxCliqueGeoV2() {
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    maxClique = init_solution;

    // reduction
    Reduction red(adjs, geometry, N, maxClique.size());

    std::vector<std::vector<int>> adjs_red = red.getAdjs_red();
    std::vector<int> bwdId = red.getBwdId();
    int redSize = red.getRedSize();
    std::vector<Node> geo_red = red.getGeo_red();

    // this adjs matrix is reused for the efficiency
    std::vector<std::vector<int>> cobipartite(redSize);

    // main iteration
    for (int u = 0; u < redSize; u++) {
        for (int v : adjs_red[u]) {
            double d = getDist(geo_red[u], geo_red[v]);
            if (d > R) continue;
            // construct co-bipartite graph
            std::vector<int> vertices;

            // find common neighbor vertices
            // for speed up, choose the smaller one
            int tar = (adjs_red[u].size() < adjs_red[v].size()) ? u : v;
            for (int k : adjs_red[tar]) {
                if (k == u || k == v) continue;
                if (getDist(geo_red[u], geo_red[k]) > d ||
                    getDist(geo_red[v], geo_red[k]) > d)
                    continue;
                vertices.push_back(k);
            }

            // construct graph
            for (int a : vertices) {
                for (int b : vertices) {
                    if (a == b) continue;
                    if (getDist(geo_red[a], geo_red[b]) > d) {
                        cobipartite[a].push_back(b);
                    }
                }
            }

            // get clique
            std::vector<int> candidate =
                getBipartiteIndep(vertices, cobipartite);
            candidate.push_back(u);
            candidate.push_back(v);

            // update solution
            if (candidate.size() > maxClique.size()) {
                maxClique = candidate;
            }

            // clear cobipartite graph
            for (int a : vertices) {
                cobipartite[a].clear();
            }
        }
    }

    // restore the vertex number
    if (maxClique.size() > init_solution.size()) {
        for (int& v : maxClique) {
            v = bwdId[v];
        }
    }
}

void HRG_CLIQUE::MaxClique::maxCliqueGeoV3() {
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    maxClique = init_solution;

    // reduction
    Reduction red(adjs, geometry, N, maxClique.size());

    std::vector<std::vector<int>> adjs_red = red.getAdjs_red();
    std::vector<int> bwdId = red.getBwdId();
    int redSize = red.getRedSize();
    std::vector<Node> geo_red = red.getGeo_red();

    // this adjs matrix is reused for the efficiency
    std::vector<std::vector<int>> cobipartite(redSize);

    // main iteration
    for (int u = 0; u < redSize; u++) {
        if (adjs_red[u].size() < maxClique.size()) continue;
        for (int v : adjs_red[u]) {
            if (adjs_red[v].size() < maxClique.size()) continue;
            double d = getDist(geo_red[u], geo_red[v]);
            if (d > R) continue;
            // construct co-bipartite graph
            std::vector<int> vertices;

            // find common neighbor vertices
            // for speed up, choose the smaller one
            int tar = (adjs_red[u].size() < adjs_red[v].size()) ? u : v;
            for (int k : adjs_red[tar]) {
                if (k == u || k == v) continue;
                if (adjs_red[k].size() < maxClique.size()) continue;
                if (getDist(geo_red[u], geo_red[k]) > d ||
                    getDist(geo_red[v], geo_red[k]) > d)
                    continue;
                vertices.push_back(k);
            }

            // construct graph
            for (int a : vertices) {
                for (int b : vertices) {
                    if (a == b) continue;
                    if (getDist(geo_red[a], geo_red[b]) > d) {
                        cobipartite[a].push_back(b);
                    }
                }
            }

            // get clique
            std::vector<int> candidate =
                getBipartiteIndep(vertices, cobipartite);
            candidate.push_back(u);
            candidate.push_back(v);

            // update solution
            if (candidate.size() > maxClique.size()) {
                maxClique = candidate;
            }

            // clear cobipartite graph
            for (int a : vertices) {
                cobipartite[a].clear();
            }
        }
    }

    // restore the vertex number
    if (maxClique.size() > init_solution.size()) {
        for (int& v : maxClique) {
            v = bwdId[v];
        }
    }
}

void HRG_CLIQUE::MaxClique::maxCliqueGeoV4() {
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    maxClique = init_solution;

    // reduction
    Reduction red(adjs, geometry, N, maxClique.size());

    std::vector<std::vector<int>> adjs_red = red.getAdjs_red();
    std::vector<int> bwdId = red.getBwdId();
    int redSize = red.getRedSize();
    std::vector<Node> geo_red = red.getGeo_red();

    // collect edges
    std::vector<Edge> edges;
    for (int u = 0; u < redSize; u++) {
        for (int v : adjs_red[u]) {
            if (u > v) continue;  // avoid duplicate (u, v)
            double d = getDist(geo_red[u], geo_red[v]);
            // if (d > R) continue;
            edges.push_back({u, v, d});
        }
    }

    // sort edges by distance in non-increasing order
    std::sort(edges.begin(), edges.end(),
              [](const Edge& a, const Edge& b) { return a.d > b.d; });

    std::vector<vector<int>> newAdjs(redSize);
    std::vector<vector<int>> cobipartite(redSize);
    std::vector<int> commonCache(redSize, 0);
    std::vector<int> neighborCache(redSize, 0);

    // main iteration
    for (int i = edges.size() - 1; i >= 0; i--) {
        int u = edges[i].u;
        int v = edges[i].v;
        newAdjs[u].push_back(v);
        newAdjs[v].push_back(u);
        if (newAdjs[u].size() < maxClique.size()) continue;
        if (newAdjs[v].size() < maxClique.size()) continue;

        // collect all common neighbor vertices
        std::vector<int> vertices;
        for (int k : newAdjs[u]) {
            if (k == v) continue;
            if (newAdjs[k].size() < maxClique.size()) continue;
            commonCache[k]++;
        }
        for (int k : newAdjs[v]) {
            if (k == u) continue;
            if (newAdjs[k].size() < maxClique.size()) continue;
            commonCache[k]++;
            if (commonCache[k] == 2) {
                vertices.push_back(k);
            }
        }

        // |vertices|+2 <= |solution|: can not be a larger clique, skip
        if (vertices.size() + 2 <= maxClique.size()) {
            // clear commonCache
            for (int k : newAdjs[u]) {
                if (k == v) continue;
                commonCache[k] = 0;
            }
            for (int k : newAdjs[v]) {
                if (k == u) continue;
                commonCache[k] = 0;
            }
            continue;
        }

        // need constructing complement graph of induced subgraph
        // time complexity: O(|V|^2)
        for (int a : vertices) {
            // check neighbors in a graph
            for (int nei : newAdjs[a]) {
                if (nei == u || nei == v) continue;
                if (commonCache[nei] == 2) {
                    neighborCache[nei]++;
                }
            }

            // find complement edges
            for (int b : vertices) {
                if (a == b) continue;
                if (!neighborCache[b]) {
                    cobipartite[a].push_back(b);
                }
            }

            // clear neighborCache
            for (int nei : newAdjs[a]) {
                if (commonCache[nei] == 2) {
                    neighborCache[nei] = 0;
                }
            }
        }

        // clear commonCache
        for (int k : newAdjs[u]) {
            commonCache[k] = 0;
        }
        for (int k : newAdjs[v]) {
            commonCache[k] = 0;
        }

        // get candidate clique
        std::vector<int> candidate = getBipartiteIndep(vertices, cobipartite);
        candidate.push_back(u);
        candidate.push_back(v);

        // update solution
        if (candidate.size() > maxClique.size()) {
            maxClique = candidate;
        }

        // clear cobipartite graph
        for (int a : vertices) {
            cobipartite[a].clear();
        }
    }

    // restore the vertex number
    if (maxClique.size() > init_solution.size()) {
        for (int& v : maxClique) {
            v = bwdId[v];
        }
    }
}
