#include "Maximumclique.hpp"

#include <cmath>
#include <vector>

#include "CNEEObuilder.hpp"
#include "Graph.hpp"
#include "MaximalClique.hpp"
#include "reduction.hpp"

std::vector<int> HRG_CLIQUE::_maxCliqueNoGeoV1(
    const std::vector<std::vector<int>>& adjs, const int N) {
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    std::vector<int> solution = init_solution;

    // reduction
    Reduction red(adjs, N, solution.size());

    std::vector<std::vector<int>> adjs_red = red.getAdjs_red();
    int redSize = red.getRedSize();

    // construct CNEEO
    CNEEObuilder CNEEObuild(adjs_red, redSize, 1);
    std::vector<std::pair<int, int>> CNEEO = CNEEObuild.getCNEEO();

    // reconstruct the graph
    // add the edges in reverse order of CNEEO
    std::vector<vector<int>> newAdjs(redSize + 1);

    // this adjs matrix is reuse for the efficiency
    std::vector<std::vector<int>> cobipartite(redSize + 1);
    std::vector<int> commonCache(redSize + 1, 0);
    std::vector<int> neighborCache(redSize + 1, 0);

    // main iteration
    for (int i = CNEEO.size() - 1; i >= 0; i--) {
        int u = CNEEO[i].first;
        int v = CNEEO[i].second;
        newAdjs[u].push_back(v);
        newAdjs[v].push_back(u);
        if (newAdjs[u].size() < solution.size()) continue;
        if (newAdjs[v].size() < solution.size()) continue;

        // collect all common neighbor vertices
        std::vector<int> vertices;
        for (int k : newAdjs[u]) {
            if (k == v) continue;
            if (newAdjs[k].size() < solution.size()) continue;
            commonCache[k]++;
        }
        for (int k : newAdjs[v]) {
            if (k == u) continue;
            if (newAdjs[k].size() < solution.size()) continue;
            commonCache[k]++;
            if (commonCache[k] == 2) {
                vertices.push_back(k);
            }
        }

        // |vertices|+2 <= |solution|: can not be a clique, skip
        if (vertices.size() + 2 <= solution.size()) {
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
        std::vector<int> candidate = getCobipMaxClique(vertices, cobipartite);
        candidate.push_back(u);
        candidate.push_back(v);

        // update solution
        if (candidate.size() > solution.size()) {
            solution = candidate;
        }

        // clear cobipartite graph
        for (int a : vertices) {
            cobipartite[a].clear();
        }
    }

    // restore the vertex number
    if (solution.size() > init_solution.size()) {
        for (int& v : solution) {
            std::vector<int> bwdId = red.getBwdId();
            v = bwdId[v];
        }
    }

    return solution;
}

std::vector<int> HRG_CLIQUE::_maxCliqueNoGeoV2(
    const std::vector<std::vector<int>>& adjs, const int N) {
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    std::vector<int> solution = init_solution;

    // reduction
    Reduction red(adjs, N, solution.size());

    std::vector<std::vector<int>> adjs_red = red.getAdjs_red();
    int redSize = red.getRedSize();

    // construct CNEEO
    CNEEObuilder CNEEObuild(adjs_red, redSize);
    std::vector<std::pair<int, int>> CNEEO = CNEEObuild.getCNEEO();

    // reconstruct the graph
    // add the edges in reverse order of CNEEO
    std::vector<vector<int>> newAdjs(redSize + 1);

    // this adjs matrix is reuse for the efficiency
    std::vector<std::vector<int>> cobipartite(redSize + 1);
    std::vector<int> commonCache(redSize + 1, 0);
    std::vector<int> neighborCache(redSize + 1, 0);

    // main iteration
    for (int i = CNEEO.size() - 1; i >= 0; i--) {
        int u = CNEEO[i].first;
        int v = CNEEO[i].second;
        newAdjs[u].push_back(v);
        newAdjs[v].push_back(u);
        if (newAdjs[u].size() < solution.size()) continue;
        if (newAdjs[v].size() < solution.size()) continue;

        // collect all common neighbor vertices
        std::vector<int> vertices;
        for (int k : newAdjs[u]) {
            if (k == v) continue;
            if (newAdjs[k].size() < solution.size()) continue;
            commonCache[k]++;
        }
        for (int k : newAdjs[v]) {
            if (k == u) continue;
            if (newAdjs[k].size() < solution.size()) continue;
            commonCache[k]++;
            if (commonCache[k] == 2) {
                vertices.push_back(k);
            }
        }

        // |vertices|+2 <= |solution|: can not be a clique, skip
        if (vertices.size() + 2 <= solution.size()) {
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
        std::vector<int> candidate = getCobipMaxClique(vertices, cobipartite);
        candidate.push_back(u);
        candidate.push_back(v);

        // update solution
        if (candidate.size() > solution.size()) {
            solution = candidate;
        }

        // clear cobipartite graph
        for (int a : vertices) {
            cobipartite[a].clear();
        }
    }

    // restore the vertex number
    if (solution.size() > init_solution.size()) {
        for (int& v : solution) {
            std::vector<int> bwdId = red.getBwdId();
            v = bwdId[v];
        }
    }

    return solution;
}

std::vector<int> HRG_CLIQUE::_maxCliqueGeoV1(
    const std::vector<std::vector<int>>& adjs, const int N,
    const std::vector<Node>& geometry, const double R) {
    std::vector<int> solution;

    // this adjs matrix is reused for the efficiency
    std::vector<std::vector<int>> cobipartite(N + 1);

    // main iteration
    for (int u = 1; u <= N; u++) {
        for (int v : adjs[u]) {
            double d = getDist(geometry[u], geometry[v]);
            // construct co-bipartite graph
            std::vector<int> vertices;

            // find common neighbor vertices
            // for speed up, choose the smaller one
            int tar = (adjs[u].size() < adjs[v].size()) ? u : v;
            for (int k : adjs[tar]) {
                if (k == u || k == v) continue;
                if (adjs[k].size() < solution.size()) continue;
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
                getCobipMaxClique(vertices, cobipartite);
            candidate.push_back(u);
            candidate.push_back(v);

            // update solution
            if (candidate.size() > solution.size()) {
                solution = candidate;
            }

            // clear cobipartite graph
            for (int a : vertices) {
                cobipartite[a].clear();
            }
        }
    }

    return solution;
}

std::vector<int> HRG_CLIQUE::_maxCliqueGeoV2(
    const std::vector<std::vector<int>>& adjs, const int N,
    const std::vector<Node>& geometry, const double R) {
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    std::vector<int> solution = init_solution;

    // reduction
    Reduction red(adjs, geometry, R, N, solution.size());

    std::vector<std::vector<int>> adjs_red = red.getAdjs_red();
    int redSize = red.getRedSize();
    std::vector<Node> geo_red = red.getGeo_red();

    // this adjs matrix is reused for the efficiency
    std::vector<std::vector<int>> cobipartite(redSize + 1);

    // main iteration
    for (int u = 1; u <= redSize; u++) {
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
                getCobipMaxClique(vertices, cobipartite);
            candidate.push_back(u);
            candidate.push_back(v);

            // update solution
            if (candidate.size() > solution.size()) {
                solution = candidate;
            }

            // clear cobipartite graph
            for (int a : vertices) {
                cobipartite[a].clear();
            }
        }
    }

    // restore the vertex number
    if (solution.size() > init_solution.size()) {
        std::vector<int> bwdId = red.getBwdId();
        for (int& v : solution) {
            v = bwdId[v];
        }
    }

    return solution;
}

std::vector<int> HRG_CLIQUE::_maxCliqueGeoV3(
    const std::vector<std::vector<int>>& adjs, const int N,
    const std::vector<Node>& geometry, const double R) {
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    std::vector<int> solution = init_solution;

    // reduction
    Reduction red(adjs, geometry, R, N, solution.size());

    std::vector<std::vector<int>> adjs_red = red.getAdjs_red();
    int redSize = red.getRedSize();
    std::vector<Node> geo_red = red.getGeo_red();

    // this adjs matrix is reused for the efficiency
    std::vector<std::vector<int>> cobipartite(redSize + 1);

    // main iteration
    for (int u = 1; u <= redSize; u++) {
        if (adjs_red[u].size() < solution.size()) continue;
        for (int v : adjs_red[u]) {
            if (adjs_red[v].size() < solution.size()) continue;
            double d = getDist(geo_red[u], geo_red[v]);
            if (d > R) continue;
            // construct co-bipartite graph
            std::vector<int> vertices;

            // find common neighbor vertices
            // for speed up, choose the smaller one
            int tar = (adjs_red[u].size() < adjs_red[v].size()) ? u : v;
            for (int k : adjs_red[tar]) {
                if (k == u || k == v) continue;
                if (adjs_red[k].size() < solution.size()) continue;
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
                getCobipMaxClique(vertices, cobipartite);
            candidate.push_back(u);
            candidate.push_back(v);

            // update solution
            if (candidate.size() > solution.size()) {
                solution = candidate;
            }

            // clear cobipartite graph
            for (int a : vertices) {
                cobipartite[a].clear();
            }
        }
    }

    // restore the vertex number
    if (solution.size() > init_solution.size()) {
        std::vector<int> bwdId = red.getBwdId();
        for (int& v : solution) {
            v = bwdId[v];
        }
    }

    return solution;
}

std::vector<int> HRG_CLIQUE::_maxCliqueGeoV4(
    const std::vector<std::vector<int>>& adjs, const int N,
    const std::vector<Node>& geometry, const double R) {
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    std::vector<int> solution = init_solution;

    // reduction
    Reduction red(adjs, geometry, R, N, solution.size());

    std::vector<std::vector<int>> adjs_red = red.getAdjs_red();
    int redSize = red.getRedSize();
    std::vector<Node> geo_red = red.getGeo_red();

    // collect edges
    std::vector<Edge> edges;
    for (int u = 1; u <= redSize; u++) {
        for (int v : adjs_red[u]) {
            if (u > v) continue;  // avoid duplicate (u, v)
            double d = getDist(geo_red[u], geo_red[v]);
            edges.push_back({u, v, d});
        }
    }

    // sort edges by distance in non-increasing order
    std::sort(edges.begin(), edges.end(),
              [](const Edge& a, const Edge& b) { return a.d > b.d; });

    std::vector<vector<int>> newAdjs(redSize + 1);
    std::vector<vector<int>> cobipartite(redSize + 1);
    std::vector<int> commonCache(redSize + 1, 0);
    std::vector<int> neighborCache(redSize + 1, 0);

    // main iteration
    for (int i = edges.size() - 1; i >= 0; i--) {
        int u = edges[i].u;
        int v = edges[i].v;
        newAdjs[u].push_back(v);
        newAdjs[v].push_back(u);
        if (newAdjs[u].size() < solution.size()) continue;
        if (newAdjs[v].size() < solution.size()) continue;

        // collect all common neighbor vertices
        std::vector<int> vertices;
        for (int k : newAdjs[u]) {
            if (k == v) continue;
            if (newAdjs[k].size() < solution.size()) continue;
            commonCache[k]++;
        }
        for (int k : newAdjs[v]) {
            if (k == u) continue;
            if (newAdjs[k].size() < solution.size()) continue;
            commonCache[k]++;
            if (commonCache[k] == 2) {
                vertices.push_back(k);
            }
        }

        // |vertices|+2 <= |solution|: can not be a larger clique, skip
        if (vertices.size() + 2 <= solution.size()) {
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
        std::vector<int> candidate = getCobipMaxClique(vertices, cobipartite);
        candidate.push_back(u);
        candidate.push_back(v);

        // update solution
        if (candidate.size() > solution.size()) {
            solution = candidate;
        }

        // clear cobipartite graph
        for (int a : vertices) {
            cobipartite[a].clear();
        }
    }

    // restore the vertex number
    if (solution.size() > init_solution.size()) {
        std::vector<int> bwdId = red.getBwdId();
        for (int& v : solution) {
            v = bwdId[v];
        }
    }

    return solution;
}

std::vector<int> HRG_CLIQUE::getMaxClique(
    const std::vector<std::vector<int>>& adjs, const int N, const int version,
    const std::vector<Node>& geometry, const double R) {
    // with geometry
    if (geometry.size()) {
        switch (version) {
            case 0:
                // optimized version (= ver.4)
                _maxCliqueGeoV4(adjs, N, geometry, R);
                break;
            case 1:
                // original version
                _maxCliqueGeoV1(adjs, N, geometry, R);
                break;
            case 2:
                // reduction version
                _maxCliqueGeoV2(adjs, N, geometry, R);
                break;
            case 3:
                // reduction + skip low degree vertices
                _maxCliqueGeoV3(adjs, N, geometry, R);
                break;
            case 4:
                // reduction + skip low degree vertices + skip + edge ordering
                _maxCliqueGeoV4(adjs, N, geometry, R);
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
                _maxCliqueNoGeoV2(adjs, N);
                break;
            case 1:
                // original version (CNEEO w/ reduction)
                _maxCliqueNoGeoV1(adjs, N);
                break;
            case 2:
                // optimized CNEEO construction vertion
                _maxCliqueNoGeoV2(adjs, N);
                break;
            default:
                assert(false);
        }
    }
}
