#include "MaximumClique.hpp"
#include "Graph.hpp"

int main(int argc, char *argv[]){
    std::ifstream ifs;
    // read graph
    std::string FILENAME1 = argv[1];
    ifs.open(FILENAME1);
    int n, m;
    ifs >> n >> m;

    std::vector<std::vector<int>> adjs(n);
    for(int i=0; i<m; i++){
        int a, b;
        ifs >> a >> b;
        adjs[a].push_back(b);
        adjs[b].push_back(a);
    }
    ifs.close();

    std::string OUTFILE = argv[2];
    int version = 0;
    if(argc > 3)
        version = atoi(argv[3]);

    // time measurement
    double start = clock();

    // solve
    HRG_CLIQUE::MaxClique solver(adjs, n, version);
    solver.run();
    std::vector<int> clique = solver.getMaxClique();

    // time measurement (ms)
    double end = clock();
    double time = (end - start) / (CLOCKS_PER_SEC / 1000);

    // output
    std::ofstream ofs;
    ofs.open(OUTFILE);
    int clique_size = clique.size();
    ofs << clique_size << std::endl;
    for(int i=0; i<clique_size; i++){
        ofs << clique[i] << " ";
    }
    ofs << std::endl;
    ofs << time << std::endl;
    ofs.close();

    return 0;
}