#pragma once

#include "tomo/geometry/kd/Tree.hpp"

#include "BuildPolicy.hpp"
#include "StaticContainer.hpp"

#include "tomo/geometry/prim/Segment.hpp"
#include "tomo/geometry/intersect/SegmentNodePointOnly.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace nearest
      {
        namespace
        {
          using prim::Segment;

          /// Defines the node with point p0 of a segment
          typedef Node<Segment> SegmentPointNode;

          /// Defines node-segment intersection strategy
          typedef intersect::SegmentNodePointOnly SegmentPointNodeIntersection;

          /// Defines the build state required for storing information in each build step
          typedef BuildState<Segment,SegmentPointNodeIntersection> SegmentBuildState;

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
              return 0.8;
            }
          };

          /// A BuildPolicy has state and defines how to make a split
          typedef BuildPolicy<SegmentBuildState,
                  SegmentSplitPos,
                  SegmentSplitCost> SegmentBuildPolicy;
        }

        /**@brief A SegmentPointTree sorts segments by their first point
          *@detail Required for segment connecting. 
                   Each Node holds a pointer to a segment, leaf node can hold several segments
         **/
        typedef Tree<SegmentNodeContainer,SegmentBuildPolicy> SegmentPointTree;
      }
    }
  }
}
