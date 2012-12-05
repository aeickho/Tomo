#pragma once

#include "StaticContainer.hpp"
#include "BuildPolicy.hpp"
#include "tomo/geometry/intersect/SegmentNode.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace object
      {
        using prim::Segment;

        typedef Node<Segment> SegmentNode;
        typedef intersect::SegmentNode SegmentNodeIntersection;
        typedef BuildState<Segment,SegmentNodeIntersection> SegmentBuildState;
        typedef StaticContainer<SegmentBuildState> SegmentNodeContainer;

        struct SegmentSplitPos
        {
          typedef Segment::scalar_type scalar_type;

          scalar_type operator()(const Segment* _segment, base::Axis _axis)
          {
            return _segment->p0()[_axis];
          }
        };

        struct SegmentSplitCost
        {
          typedef Segment::scalar_type scalar_type;
          scalar_type operator()(const Segment* _v)
          {
            return 0.333;
          }
        };

        typedef BuildPolicy<SegmentBuildState,
                            SegmentSplitPos,
                            SegmentSplitCost> SegmentBuildPolicy;

        typedef Tree<SegmentNodeContainer,SegmentBuildPolicy> SegmentTree;
      }
    }
  }
}
