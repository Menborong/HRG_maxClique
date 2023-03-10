#pragma once
#include <vector>

#include "Graph.hpp"
#include "VertexCover.hpp"

namespace HRG_CLIQUE {

class MaxClique {
   public:
    MaxClique(const std::vector<std::vector<int>>& adjs, const int N,
              const int version = 0, const std::vector<Node>& geometry = {},
              const double R = 0.0)
        : adjs(adjs),
          N(N),
          version(version),
          geometry(geometry),
          R(R),
          hasGeo(geometry.size() != 0) {}
    void run();
    std::vector<int> getMaxClique() const { return maxClique; }
    std::vector<pair<int, int>> getFailedEdges() const { return failedEdges; }

   private:
    const std::vector<std::vector<int>>& adjs;
    const int N;
    const int version;
    const std::vector<Node>& geometry;
    const double R;
    const bool hasGeo;

    std::vector<int> maxClique;
    std::vector<pair<int, int>> failedEdges;

    void maxCliqueNoGeoV1();
    void maxCliqueNoGeoV2();
    void maxCliqueGeoV1();
    void maxCliqueGeoV2();
    void maxCliqueGeoV3();
    void maxCliqueGeoV4();
};
}  // namespace HRG_CLIQUE