#pragma once

#include "tomo/geometry/prim/ConnectableSegment.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      NodeIntersectResult intersect(prim::ConnectableSegment& _lineSegment,
                                    NodeGeometry<prim::ConnectableSegment>& _node)
      {
        return NodeIntersectResult(_lineSegment[1][_node.axis()] <= _node.splitPos(),
                                   _lineSegment[1][_node.axis()] >= _node.splitPos());
      }

    }
  }
}
