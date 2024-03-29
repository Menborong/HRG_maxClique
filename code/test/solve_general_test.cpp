#include <fstream>

#include "Graph.hpp"
#include "MaximalClique.hpp"
#include "MaximumClique.hpp"
#include "Reduction.hpp"

// input_graph output_filename (version)

int main(int argc, char *argv[]) {
    std::ifstream ifs;
    // read graph
    std::string FILENAME1 = argv[1];
    ifs.open(FILENAME1);
    int n, m;
    ifs >> n >> m;

    std::vector<std::vector<int>> adjs(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        ifs >> a >> b;
        adjs[a].push_back(b);
        adjs[b].push_back(a);
    }
    ifs.close();

    std::string OUTFILE = argv[2];
    int version = 0;
    if (argc > 3) version = atoi(argv[3]);

    // time measurement
    double start = clock();

    // solve
    HRG_CLIQUE::MaxClique solver(adjs, n, version);
    solver.run();
    std::vector<int> clique = solver.getMaxClique();

    // time measurement (ms)
    double end = clock();
    double time = (end - start) / (CLOCKS_PER_SEC / 1000);

    // reduction test
    std::vector<int> init_clique = HRG_CLIQUE::getMaximalClique(adjs, n);
    HRG_CLIQUE::Reduction red(adjs, n, init_clique.size());

    // output
    std::ofstream ofs;
    ofs.open(OUTFILE, std::ios::app);

    ofs << n << std::endl;  // input size

    ofs << init_clique.size() << " " << red.getRedSize() << std::endl;

    int clique_size = clique.size();
    ofs << clique_size << std::endl;
    for (int i = 0; i < clique_size; i++) {
        ofs << clique[i] << " ";
    }
    ofs << std::endl;

    int leftV = 0;
    int leftE = 0;

    std::vector<std::pair<int, int>> fails = solver.getFailedEdges();
    std::vector<int> chkLeftV(n);
    leftE = fails.size();
    for (auto [u, v] : fails) {
        if (!chkLeftV[u]) {
            leftV++;
            chkLeftV[u] = 1;
        }
        if (!chkLeftV[v]) {
            leftV++;
            chkLeftV[v] = 1;
        }
    }

    ofs << leftV << ' ' << leftE << std::endl;  // left graph size

    ofs << time << std::endl;  // running time

    ofs.close();

    return 0;
}