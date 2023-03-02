#include <bits/stdc++.h>

double calculateRadius(int n, double alpha, int deg) {
    return 2 * log(n * 2 * alpha * alpha * (1 / M_PI) /
                   (deg * (alpha - 0.5) * (alpha - 0.5)));
}

int main(int argc, char* argv[]){
    int N = atoi(argv[1]);
    double alpha = atof(argv[2]);
    double deg = atof(argv[3]);

    auto R = calculateRadius(N, alpha, deg);

    std::string out_name(argv[4]);
    std::ofstream fout(out_name);
    fout << std::fixed;
    fout.precision(17);
    fout << R << '\n';

    return 0;
}