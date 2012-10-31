#pragma once
#include "PointNode.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace intersect
    {
      struct SegmentNode
      {
        typedef prim::Segment Segment;
        typedef kd::NodeGeometry<Segment::model_type> NodeGeometry;

        kd::NodeIntersectResult operator()(const Segment& _segment,
                                           const NodeGeometry& _node)
        {
          return PointNode<point_type>(_segment[0],_node);
        }
      };
    }
  }
}
