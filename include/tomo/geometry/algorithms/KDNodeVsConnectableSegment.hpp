#pragma once

#include "tomo/geometry/prim/ConnectableSegment.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace algorithms
    {
      aux::KDNodeIntersectResult intersect(prim::ConnectableSegment& _lineSegment,
                                           aux::KDNodeGeometry<prim::ConnectableSegment>& _node)
      {
        return aux::KDNodeIntersectResult(_lineSegment[1][_node.axis()] <= _node.splitPos(),
                                          _lineSegment[1][_node.axis()] >= _node.splitPos());
      }

    }
  }
}
