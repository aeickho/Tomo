#pragma once
#include "PointNode.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace intersect
    {
      struct SegmentNodePointOnly
      {
        typedef prim::Segment segment_type;
        typedef segment_type::point_type point_type;
        typedef kd::NodeGeometry<segment_type::model_type> node_geometry_type;

        kd::NodeIntersectResult operator()(const segment_type& _segment,
                                           const node_geometry_type& _node)
        {
          return PointNode<point_type>()(_segment[0],_node);
        }
      };
    }
  }
}
