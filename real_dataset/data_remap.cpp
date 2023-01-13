#include <bits/stdc++.h>
using namespace std;

int main(){
    string file_name;
    cin >> file_name;
    ifstream fin(file_name);
    int N, M;
    fin >> N >> M;

    int op;
    cin >> op;

    vector<pair<int, int>> edges;
    for(int i=0; i<M; i++){
        int u, v;
        fin >> u >> v;
        edges.push_back({u, v});
        if(op){
            double w;
            fin >> w;
            // ignore
        }
    }

    map<int, int> comp;
    int cnt = 1;
    for(auto [u, v]: edges){
        if(comp.find(u) == comp.end()){
            comp[u] = cnt++;
        }
        if(comp.find(v) == comp.end()){
            comp[v] = cnt++;
        }
    }

    string out_name = file_name + ".remap";
    ofstream fout(out_name);
    fout << N << " " << M << endl;
    for(auto [u, v]: edges){
        fout << comp[u] << " " << comp[v] << endl;
    }

    return 0;
}