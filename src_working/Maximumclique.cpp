#include "Maximumclique.hpp"
#include "reduction.hpp"
#include "CNEEObuilder.hpp"
#include <vector>
#include <cmath>

double HRG_CLIQUE::getDist(Node &a, Node &b)
{
    if(a.phi == b.phi) return abs(a.r-b.r);
    double delta = M_PI - abs(M_PI - abs(a.phi - b.phi));
    double tmp = M_PI;
    double d = acosh(cosh(a.r)*cosh(b.r) - sinh(a.r)*sinh(b.r)*cos(delta));
    if(d != d){
        // cout << "Fail to calculate distance" << '\n';
        // cout << a.r << ' ' << a.phi << ' ' << b.r << ' ' << b.phi << '\n';
        return abs(a.r - b.r);
    } 
    return d;
}

std::vector<int> HRG_CLIQUE::getMaxClique(std::vector<std::vector<int>> &adjs, std::vector<Node> &geometry, double R, int N)
{
    std::vector<int> solution;

    // this adjs matrix is reused for the efficiency
    std::vector<std::vector<int>> cobipartite(N+1);

    // main iteration
    for(int u=1; u<=N; u++){
        for(int v: adjs[u]){
            double d = getDist(geometry[u], geometry[v]);
            // construct co-bipartite graph
            std::vector<int> vertices;

            // find common neighbor vertices
            int tar = (adjs[u].size() < adjs[v].size()) ? u : v; // for speed up, choose the smaller one
            for(int k: adjs[tar]){
                if(k == u || k == v) continue;
                if(adjs[k].size() < solution.size()) continue;
                if(getDist(geometry[u], geometry[k]) > d || getDist(geometry[v], geometry[k]) > d) continue;
                vertices.push_back(k);
            }

            // construct graph
            for(int a: vertices){
                for(int b: vertices){
                    if(a == b) continue;
                    if(getDist(geometry[a], geometry[b]) > d){
                        cobipartite[a].push_back(b);
                    }
                }
            }

            // get clique
            std::vector<int> candidate = getCobipMaxClique(vertices, cobipartite);
            candidate.push_back(u);
            candidate.push_back(v);

            // update solution
            if(candidate.size() > solution.size()){
                solution = candidate;
            }

            // clear cobipartite graph
            for(int a: vertices){
                cobipartite[a].clear();
            }

        }
    }

    return solution;
}

std::vector<int> HRG_CLIQUE::getMaxCliqueRed(std::vector<std::vector<int>> &adjs, std::vector<Node> &geometry, double R, int N)
{
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    std::vector<int> solution = init_solution;

    // reduction
    std::vector<int> red = reduction(adjs, N, solution.size());

    // reconstruct the graph
    int newN = red.size();
    std::vector<int> newId(N+1, 0);
    std::vector<int> bckId(newN+1, 0);
    for(int i=0; i<newN; i++){
        newId[red[i]] = i+1;
        bckId[i+1] = red[i];
    }

    std::vector<vector<int>> newAdjs(newN+1);
    for(int u=1; u<=N; u++){
        if(!newId[u]) continue;
        for(int v: adjs[u]){
            if(!newId[v]) continue;
            newAdjs[newId[u]].push_back(newId[v]);
        }
    }

    // this adjs matrix is reused for the efficiency
    std::vector<std::vector<int>> cobipartite(newN+1);

    // main iteration
    for(int u=1; u<=newN; u++){
        for(int v: newAdjs[u]){
            double d = getDist(geometry[bckId[u]], geometry[bckId[v]]);
            if(d > R) continue;
            // construct co-bipartite graph
            std::vector<int> vertices;

            // find common neighbor vertices
            int tar = (newAdjs[u].size() < newAdjs[v].size()) ? u : v; // for speed up, choose the smaller one
            for(int k: newAdjs[tar]){
                if(k == u || k == v) continue;
                if(getDist(geometry[bckId[u]], geometry[bckId[k]]) > d || getDist(geometry[bckId[v]], geometry[bckId[k]]) > d) continue;
                vertices.push_back(k);
            }

            // construct graph
            for(int a: vertices){
                for(int b: vertices){
                    if(a == b) continue;
                    if(getDist(geometry[bckId[a]], geometry[bckId[b]]) > d){
                        cobipartite[a].push_back(b);
                    }
                }
            }

            // get clique
            std::vector<int> candidate = getCobipMaxClique(vertices, cobipartite);
            candidate.push_back(u);
            candidate.push_back(v);

            // update solution
            if(candidate.size() > solution.size()){
                solution = candidate;
            }

            // clear cobipartite graph
            for(int a: vertices){
                cobipartite[a].clear();
            }

        }
    }
    
    // restore the vertex number
    if(solution.size() > init_solution.size()){
        for(int &v: solution){
            v = red[v-1];
        }
    }

    return solution;
}

std::vector<int> HRG_CLIQUE::getMaxCliqueSkip(std::vector<std::vector<int>> &adjs, std::vector<Node> &geometry, double R, int N)
{

    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    std::vector<int> solution = init_solution;

    // reduction
    std::vector<int> red = reduction(adjs, N, solution.size());

    // reconstruct the graph
    int newN = red.size();
    std::vector<int> newId(N+1, 0);
    std::vector<int> bckId(newN+1, 0);
    for(int i=0; i<newN; i++){
        newId[red[i]] = i+1;
        bckId[i+1] = red[i];
    }

    std::vector<vector<int>> newAdjs(newN+1);
    for(int u=1; u<=N; u++){
        if(!newId[u]) continue;
        for(int v: adjs[u]){
            if(!newId[v]) continue;
            newAdjs[newId[u]].push_back(newId[v]);
        }
    }

    // this adjs matrix is reused for the efficiency
    std::vector<std::vector<int>> cobipartite(newN+1);

    // main iteration
    for(int u=1; u<=newN; u++){
        if(newAdjs[u].size() < solution.size()) continue;
        for(int v: newAdjs[u]){
            if(newAdjs[v].size() < solution.size()) continue;
            double d = getDist(geometry[bckId[u]], geometry[bckId[v]]);
            if(d > R) continue;
            // construct co-bipartite graph
            std::vector<int> vertices;

            // find common neighbor vertices
            int tar = (newAdjs[u].size() < newAdjs[v].size()) ? u : v; // for speed up, choose the smaller one
            for(int k: newAdjs[tar]){
                if(k == u || k == v) continue;
                if(newAdjs[k].size() < solution.size()) continue;
                if(getDist(geometry[bckId[u]], geometry[bckId[k]]) > d || getDist(geometry[bckId[v]], geometry[bckId[k]]) > d) continue;
                vertices.push_back(k);
            }

            // |vertices|+2 <= |solution|: can not be a larger clique, skip
            if(vertices.size() + 2 <= solution.size()){
                continue;
            }

            // construct graph
            for(int a: vertices){
                for(int b: vertices){
                    if(a == b) continue;
                    if(getDist(geometry[bckId[a]], geometry[bckId[b]]) > d){
                        cobipartite[a].push_back(b);
                    }
                }
            }

            // get clique
            std::vector<int> candidate = getCobipMaxClique(vertices, cobipartite);
            candidate.push_back(u);
            candidate.push_back(v);

            // update solution
            if(candidate.size() > solution.size()){
                solution = candidate;
            }

            // clear cobipartite graph
            for(int a: vertices){
                cobipartite[a].clear();
            }

        }
    }
    
    // restore the vertex number
    if(solution.size() > init_solution.size()){
        for(int &v: solution){
            v = red[v-1];
        }
    }

    return solution;

}

std::vector<int> HRG_CLIQUE::getMaxCliqueOpt(std::vector<std::vector<int>> &adjs, std::vector<Node> &geometry, double R, int N)
{
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    std::vector<int> solution = init_solution;

    // reduction
    std::vector<int> red = reduction(adjs, N, solution.size());

    // reconstruct the graph
    int newN = red.size();
    std::vector<int> newId(N+1, 0);
    std::vector<int> bckId(newN+1, 0);
    for(int i=0; i<newN; i++){
        newId[red[i]] = i+1;
        bckId[i+1] = red[i];
    }

    std::vector<Edge> edges;
    for(int u=1; u<=N; u++){
        if(!newId[u]) continue;
        for(int v: adjs[u]){
            if(!newId[v]) continue;
            if(newId[u] > newId[v]) continue; // avoid duplicate (u, v)
            double d = getDist(geometry[u], geometry[v]);
            edges.push_back({newId[u], newId[v], d});
        }
    }

    // sort edges by distance in non-increasing order
    std::sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b){
        return a.d > b.d;
    });

    std::vector<vector<int>> newAdjs(newN+1);
    // this adjs matrix is reuse for the efficiency
    std::vector<std::vector<int>> cobipartite(newN+1);
    std::vector<int> commonCache(newN+1, 0);
    std::vector<int> neighborCache(newN+1, 0);

    // std::set<std::pair<int, int>> edgeSet;

    // main iteration
    for(int i=edges.size()-1; i>=0; i--){
        int u = edges[i].u;
        int v = edges[i].v;
        newAdjs[u].push_back(v);
        newAdjs[v].push_back(u);
        if(newAdjs[u].size() < solution.size()) continue;
        if(newAdjs[v].size() < solution.size()) continue;

        // collect all common neighbor vertices
        std::vector<int> vertices;
        for(int k: newAdjs[u]){
            if(k == v) continue;
            if(newAdjs[k].size() < solution.size()) continue;
            commonCache[k]++;
        }
        for(int k: newAdjs[v]){
            if(k == u) continue;
            if(newAdjs[k].size() < solution.size()) continue;
            commonCache[k]++;
            if(commonCache[k] == 2){
                vertices.push_back(k);
            }
        }

        // |vertices|+2 <= |solution|: can not be a larger clique, skip
        if(vertices.size() + 2 <= solution.size()){
            // clear commonCache
            for(int k: newAdjs[u]){
                if(k == v) continue;
                commonCache[k] = 0;
            }
            for(int k: newAdjs[v]){
                if(k == u) continue;
                commonCache[k] = 0;
            }
            continue;
        }

        // need constructing complement graph of induced subgraph
        // time complexity: O(|V|^2)
        for(int a: vertices){
            // check neighbors in a graph
            for(int nei: newAdjs[a]){
                if(nei == u || nei == v) continue;
                if(commonCache[nei] == 2){
                    neighborCache[nei]++;
                }
            }

            // find complement edges
            for(int b: vertices){
                if(a == b) continue;
                if(!neighborCache[b]){
                    cobipartite[a].push_back(b);
                }
            }

            // clear neighborCache
            for(int nei: newAdjs[a]){
                if(commonCache[nei] == 2){
                    neighborCache[nei] = 0;
                }
            }
        }

        // clear commonCache
        for(int k: newAdjs[u]){
            commonCache[k] = 0;
        }
        for(int k: newAdjs[v]){
            commonCache[k] = 0;
        }

        // get candidate clique
        std::vector<int> candidate = getCobipMaxClique(vertices, cobipartite);
        candidate.push_back(u);
        candidate.push_back(v);

        // update solution
        if(candidate.size() > solution.size()){
            solution = candidate;
        }

        // clear cobipartite graph
        for(int a: vertices){
            cobipartite[a].clear();
        }
    }

    // restore the vertex number
    if(solution.size() > init_solution.size()){
        for(int &v: solution){
            v = red[v-1];
        }
    }

    return solution;
}

std::vector<int> HRG_CLIQUE::getMaxClique(std::vector<std::vector<int>> &adjs, int N)
{
    // initalize solution
    std::vector<int> init_solution = getMaximalClique(adjs, N);
    std::vector<int> solution = init_solution;

    // reduction
    std::vector<int> red = reduction(adjs, N, solution.size());
    std::cout << "initial size " << N << '\n';
    std::cout << "reduction: " << red.size() << '\n';

    // reconstruct the graph
    int newN = red.size();
    std::vector<int> newId(N+1, 0);
    std::vector<int> bckId(newN+1, 0);
    for(int i=0; i<newN; i++){
        newId[red[i]] = i+1;
        bckId[i+1] = red[i];
    }

    std::vector<vector<int>> newAdjs(newN+1);
    for(int u=1; u<=N; u++){
        if(!newId[u]) continue;
        for(int v: adjs[u]){
            if(!newId[v]) continue;
            newAdjs[newId[u]].push_back(newId[v]);
        }
    }

    // construct CNEEO
    std::vector<pair<int, int>> CNEEO = getCNEEO(newAdjs, newN);

    // reconstruct the graph
    // add the edges in reverse order of CNEEO
    for(int i=1; i<=newN; i++){
        newAdjs[i].clear();
    }

    // this adjs matrix is reuse for the efficiency
    std::vector<std::vector<int>> cobipartite(newN+1);
    std::vector<int> commonCache(newN+1, 0);
    std::vector<int> neighborCache(newN+1, 0);

    // std::set<std::pair<int, int>> edgeSet;

    // main iteration
    for(int i=CNEEO.size()-1; i>=0; i--){
        int u = CNEEO[i].first;
        int v = CNEEO[i].second;
        newAdjs[u].push_back(v);
        newAdjs[v].push_back(u);
        if(newAdjs[u].size() < solution.size()) continue;
        if(newAdjs[v].size() < solution.size()) continue;

        // collect all common neighbor vertices
        std::vector<int> vertices;
        for(int k: newAdjs[u]){
            if(k == v) continue;
            if(newAdjs[k].size() < solution.size()) continue;
            commonCache[k]++;
        }
        for(int k: newAdjs[v]){
            if(k == u) continue;
            if(newAdjs[k].size() < solution.size()) continue;
            commonCache[k]++;
            if(commonCache[k] == 2){
                vertices.push_back(k);
            }
        }

        // |vertices|+2 <= |solution|: can not be a clique, skip
        if(vertices.size() + 2 <= solution.size()){
            // clear commonCache
            for(int k: newAdjs[u]){
                if(k == v) continue;
                commonCache[k] = 0;
            }
            for(int k: newAdjs[v]){
                if(k == u) continue;
                commonCache[k] = 0;
            }
            continue;
        }

        // need constructing complement graph of induced subgraph
        // time complexity: O(|V|^2)
        for(int a: vertices){
            // check neighbors in a graph
            for(int nei: newAdjs[a]){
                if(nei == u || nei == v) continue;
                if(commonCache[nei] == 2){
                    neighborCache[nei]++;
                }
            }

            // find complement edges
            for(int b: vertices){
                if(a == b) continue;
                if(!neighborCache[b]){
                    cobipartite[a].push_back(b);
                }
            }

            // clear neighborCache
            for(int nei: newAdjs[a]){
                if(commonCache[nei] == 2){
                    neighborCache[nei] = 0;
                }
            }
        }

        // clear commonCache
        for(int k: newAdjs[u]){
            commonCache[k] = 0;
        }
        for(int k: newAdjs[v]){
            commonCache[k] = 0;
        }

        // get candidate clique
        std::vector<int> candidate = getCobipMaxClique(vertices, cobipartite);
        candidate.push_back(u);
        candidate.push_back(v);

        // update solution
        if(candidate.size() > solution.size()){
            solution = candidate;
        }

        // clear cobipartite graph
        for(int a: vertices){
            cobipartite[a].clear();
        }
    }

    // restore the vertex number
    if(solution.size() > init_solution.size()){
        for(int &v: solution){
            v = red[v-1];
        }
    }

    return solution;

}
