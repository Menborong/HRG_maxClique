#include <bits/stdc++.h>

double calculateRadius(int n, double alpha, double T, int deg) {
    return 2 * log(n * 2 * alpha * alpha * (T == 0 ? 1 / M_PI : T / sin(M_PI * T)) /
                   (deg * (alpha - 0.5) * (alpha - 0.5)));
}

int main(int argc, char* argv[]){
    int N = atoi(argv[1]);
    double alpha = atof(argv[2]);
    double T = atof(argv[3]);
    double deg = atof(argv[4]);

    auto R = calculateRadius(N, alpha, T, deg);

    std::string out_name(argv[5]);
    std::ofstream fout(out_name);
    fout << std::fixed;
    fout.precision(17);
    fout << N << '\n';
    fout << alpha << '\n';
    fout << T << '\n';
    fout << deg << '\n';
    fout << R << '\n';

    return 0;
}