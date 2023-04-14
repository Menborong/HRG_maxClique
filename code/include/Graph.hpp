#pragma once
#include <gsl/gsl_math.h>

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
double getDist(const Node& a, const Node& b);

}  // namespace HRG_CLIQUE