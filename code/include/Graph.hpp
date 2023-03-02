#pragma once
#include <cmath>

namespace HRG_CLIQUE {

struct Node {
    double r;
    double phi;
    Node() : r(0.0), phi(0.0) {}
    Node(double r, double phi) : r(r), phi(phi) {}
};

struct Edge {
    int u, v;
    double d;
    Edge() : u(0), v(0), d(0.0) {}
    Edge(int u, int v, double d) : u(u), v(v), d(d) {}
};

/// @brief calculate the distance between two nodes in the hyperbolic plane
/// @param a,b input nodes
/// @return distance between a and b
double HRG_CLIQUE::getDist(const Node& a, const Node& b) {
    if (a.phi == b.phi) return abs(a.r - b.r);
    double delta = M_PI - abs(M_PI - abs(a.phi - b.phi));
    double tmp = M_PI;
    double d =
        acosh(cosh(a.r) * cosh(b.r) - sinh(a.r) * sinh(b.r) * cos(delta));
    if (d != d) {
        // cout << "Fail to calculate distance" << '\n';
        // cout << a.r << ' ' << a.phi << ' ' << b.r << ' ' << b.phi << '\n';
        return abs(a.r - b.r);
    }
    return d;
}

}  // namespace HRG_CLIQUE