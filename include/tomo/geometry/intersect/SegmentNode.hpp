#pragma once

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

        NodeIntersectResult operator()(const Segment& _segment,
                                       const NodeGeometry& _node)
        {

        
        }
      };
    }
  }
}
