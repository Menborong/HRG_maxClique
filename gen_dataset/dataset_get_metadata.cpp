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
    std::string out_name(argv[5]);

    auto R = calculateRadius(N, alpha, T, deg);

    std::stringstream ss;
    ss << std::fixed;
    ss.precision(17);
    ss << R << '\n';

    std::ifstream fin(out_name);
    ss << fin.rdbuf();
    fin.close();

    std::ofstream fout(out_name, std::ios::out);
    fout << ss.rdbuf();

    return 0;
}