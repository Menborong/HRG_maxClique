#pragma once
#include <vector>

#include "Graph.hpp"
#include "reduction.hpp"
#include "vertexCover.hpp"

namespace HRG_CLIQUE {

std::vector<int> _maxCliqueNoGeoV1(const std::vector<std::vector<int>>& adjs,
                                   const int N);
std::vector<int> _maxCliqueNoGeoV2(const std::vector<std::vector<int>>& adjs,
                                   const int N);

std::vector<int> _maxCliqueGeoV1(const std::vector<std::vector<int>>& adjs,
                                 const int N, const std::vector<Node>& geometry,
                                 const double R);
std::vector<int> _maxCliqueGeoV2(const std::vector<std::vector<int>>& adjs,
                                 const int N, const std::vector<Node>& geometry,
                                 const double R);
std::vector<int> _maxCliqueGeoV3(const std::vector<std::vector<int>>& adjs,
                                 const int N, const std::vector<Node>& geometry,
                                 const double R);
std::vector<int> _maxCliqueGeoV4(const std::vector<std::vector<int>>& adjs,
                                 const int N, const std::vector<Node>& geometry,
                                 const double R);

std::vector<int> getMaxClique(const std::vector<std::vector<int>>& adjs,
                              const int N, const int version = 0,
                              const std::vector<Node>& geometry = {},
                              const double R = 0.0);

}  // namespace HRG_CLIQUE