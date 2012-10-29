#pragma once

#include "tomo/geometry/prim/Segment.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd 
    {
      namespace visitor
      {
        struct SegmentNodeIntersectPointOnly
        {
          typedef prim::Segment Segment;

          NodeIntersectResult operator()(const Segment& _segment,
                                         const NodeGeometry<Segment::model_type>& _node)
          {
            return NodeIntersectResult(_segment[0][_node.axis()] <= _node.splitPos(),
                                       _segment[0][_node.axis()] >= _node.splitPos());
          }
        };
      }
    }
  }
}
