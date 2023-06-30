#include "Graph.hpp"

#include <gsl/gsl_math.h>

double HRG_CLIQUE::getDist(const Node& a, const Node& b) {
    if (a.phi == b.phi) return abs(a.r - b.r);
    double delta = M_PI - abs(M_PI - abs(a.phi - b.phi));
    double d =
        acosh(cosh(a.r) * cosh(b.r) - sinh(a.r) * sinh(b.r) * cos(delta));
    if (d != d) {
        // cout << "Fail to calculate distance" << '\n';
        // cout << a.r << ' ' << a.phi << ' ' << b.r << ' ' << b.phi << '\n';
        return abs(a.r - b.r);
    }
    return d;
}