#include <vector>
#include <queue>
#include "reduction.hpp"

// using namespace std;
// using vInt = vector<int>;
// using matInt = vector<vInt>;

namespace HRG_CLIQUE{

    // get the maximal clique of the graph
    // using greedy algorithm with the order of degree in descending order
    // time complexity: O(N+M)
    std::vector<int> getMaximalClique(std::vector<std::vector<int>>& adjs, int N){
        // sort the vertices by degree in descending order using counting sort
        // time complexity: O(N)
        std::vector<std::vector<int>> degrees(N+1);
        for(int i=1; i<=N; i++){
            degrees[adjs[i].size()].push_back(i);
        }
        std::vector<int> sorted;
        for(int i=N; i>=0; i--){
            for(int j=0; j<degrees[i].size(); j++){
                sorted.push_back(degrees[i][j]);
            }
        }

        // greedy algorithm
        std::vector<int> clique;
        std::vector<int> numCliqueNeighbor(N+1, 0);

        for(int i=0; i<N; i++){
            int v = sorted[i];
            if(numCliqueNeighbor[v] == clique.size()){
                clique.push_back(v);
                for(int j=0; j<adjs[v].size(); j++){
                    numCliqueNeighbor[adjs[v][j]]++;
                }
            }
        }

        return clique;
    }

    // get the new vertex set after reduction
    // by deleting all vertices with degree lower than redSize
    // until there is no such a vertex.
    // time complexity: O(N+M)
    std::vector<int> reduction(std::vector<std::vector<int>>& adjs, int N, int redSize){       
        // BFS scheme
        std::queue<int> q;
        std::vector<int> degree(N+1);
        for(int i=1; i<=N; i++){
            degree[i] = adjs[i].size();
            if(degree[i] < redSize){
                q.push(i);
            }
        }

        while(!q.empty()){
            int tar = q.front();
            q.pop();
            for(int nxt: adjs[tar]){
                if(degree[nxt] < redSize){ // already deleted
                    continue;
                }
                degree[nxt]--;
                if(degree[nxt] < redSize){
                    q.push(nxt);
                }
            }
        }

        std::vector<int> remains;
        for(int i=1; i<=N; i++){
            if(degree[i] >= redSize){
                remains.push_back(i);
            }
        }

        return remains;
    }
}