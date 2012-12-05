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
        namespace
        {
          using prim::Segment;

          /// Defines the node with a segment
          typedef Node<Segment> SegmentNode;

          /// Defines node-segment intersection strategy
          typedef intersect::SegmentNode SegmentNodeIntersection;

          /// Defines the build state required for storing information in each build step
          typedef BuildState<Segment,SegmentNodeIntersection> SegmentBuildState;

          /// Defines how nodes and primitives are stored in the tree
          typedef StaticContainer<SegmentBuildState> SegmentNodeContainer;

          /// Functor for determining a split position from a primitive
          struct SegmentSplitPos
          {
            typedef Segment::scalar_type scalar_type;

            scalar_type operator()(const Segment* _segment, base::Axis _axis)
            {
              return _segment->p0()[_axis];
            }
          };

          /// Functor for determining the cost of each primitive when making a split
          struct SegmentSplitCost
          {
            typedef Segment::scalar_type scalar_type;
            scalar_type operator()(const Segment* _v)
            {
              return 0.333;
            }
          };

          /// A BuildPolicy has state and defines how to make a split
          typedef BuildPolicy<SegmentBuildState,
                  SegmentSplitPos,
                  SegmentSplitCost> SegmentBuildPolicy;
        }

        /// A Tree which holds pointers to segments in its leaves
        typedef Tree<SegmentNodeContainer,SegmentBuildPolicy> SegmentTree;
      }
    }
  }
}
