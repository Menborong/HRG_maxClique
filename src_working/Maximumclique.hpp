#pragma once
#include <vector>

#include "reduction.hpp"
#include "vertexCover.hpp"

namespace HRG_CLIQUE {
struct Node {
    double r;
    double phi;
};

struct Edge {
    int u, v;
    double d;
};

double getDist(Node& a, Node& b);

// // given a hyperbolic random graph and corresponding geometry, find the
// maximum
// // clique
// std::vector<int> getMaxClique(std::vector<std::vector<int>>& adjs,
//                               std::vector<Node>& geometry, double R, int N);

// // getMaxClique + reduction
// std::vector<int> getMaxCliqueRed(std::vector<std::vector<int>>& adjs,
//                                  std::vector<Node>& geometry, double R, int
//                                  N);

// // getMaxClique + reduction + skip vertices with low degree
// std::vector<int> getMaxCliqueSkip(std::vector<std::vector<int>>& adjs,
//                                   std::vector<Node>& geometry, double R, int
//                                   N);

// // getMaxClique + reduction + skip vertices with low degree + edge ordering
// std::vector<int> getMaxCliqueOpt(std::vector<std::vector<int>>& adjs,
//                                  std::vector<Node>& geometry, double R, int
//                                  N);

// // given a hyperbolic random graph but no geometry, find the maximum clique
// (use
// // CNEEO)
// std::vector<int> getMaxClique(std::vector<std::vector<int>>& adjs, int N);

std::vector<int> _getMaxCliqueGeoVer1(const std::vector<std::vector<int>>& adjs,
                                      const int N,
                                      const std::vector<Node>& geometry,
                                      const double R);

std::vector<int> _getMaxCliqueGeoVer2(const std::vector<std::vector<int>>& adjs,
                                      const int N,
                                      const std::vector<Node>& geometry,
                                      const double R);

std::vector<int> getMaxClique(const std::vector<std::vector<int>>& adjs,
                              const int N, const int version = 0,
                              const std::vector<Node>& geometry = {},
                              const double R = 0.0);

}  // namespace HRG_CLIQUE