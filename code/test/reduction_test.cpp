#include <fstream>
#include "MaximalClique.hpp"
#include "Reduction.hpp"
#include "Graph.hpp"

int main(int argc, char *argv[]) {
    (void)argc;
    std::ifstream ifs;
    // read graph
    std::string FILENAME1 = argv[1];
    ifs.open(FILENAME1);
    int n, m;
    ifs >> n >> m;

    std::vector<std::vector<int>> adjs(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        ifs >> a >> b;
        adjs[a].push_back(b);
        adjs[b].push_back(a);
    }
    ifs.close();

    std::string OUTFILE = argv[2];

    // get initial solution (maximal)
    std::vector<int> clique = HRG_CLIQUE::getMaximalClique(adjs, n);
    int initial_size = clique.size();
    // solve
    HRG_CLIQUE::Reduction red(adjs, n, initial_size);

    // output
    std::ofstream ofs;
    ofs.open(OUTFILE);
    ofs << red.getRedSize() << std::endl;
    ofs.close();

    return 0;
}