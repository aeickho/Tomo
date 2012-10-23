#pragma once

#include "tomo/geometry/prim/Segment.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      NodeIntersectResult intersect(const prim::Segment& _segment,
                                    const NodeGeometry<typename PRIMITIVE::model_type>& _node)
      {
        return NodeIntersectResult(_lineSegment[1][_node.axis()] <= _node.splitPos(),
                                   _lineSegment[1][_node.axis()] >= _node.splitPos());
      }
    }
  }
}
