#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]){
    std::string in_name = argv[1];
    std::string out_name = argv[2];

    ifstream fin(in_name);
    int N, M;
    fin >> N >> M;

    vector<pair<int, int>> edges;
    for(int i=0; i<M; i++){
        int u, v;
        fin >> u >> v;
        edges.push_back({u, v});
    }

    map<int, int> comp;
    int cnt = 0;
    for(auto [u, v]: edges){
        if(comp.find(u) == comp.end()){
            comp[u] = cnt++;
        }
        if(comp.find(v) == comp.end()){
            comp[v] = cnt++;
        }
    }

    ofstream fout(out_name);
    fout << N << " " << M << '\n';
    for(auto [u, v]: edges){
        fout << comp[u] << " " << comp[v] << '\n';
    }

    return 0;
}