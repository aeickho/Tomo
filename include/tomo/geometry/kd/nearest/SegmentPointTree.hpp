#pragma once

#include "Node.hpp"
#include "BuildState.hpp"
#include "BuildPolicy.hpp"

#include "tomo/geometry/kd/Tree.hpp"
#include "tomo/geometry/kd/StaticContainer.hpp"
#include "tomo/geometry/kd/BuildState.hpp"
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
        typedef intersect::SegmentNodePointOnly SegmentPointNodeIntersection;
        typedef Node<prim::Segment> SegmentPointNode;

        struct SegmentPointBuildState : BuildState<prim::Segment,SegmentPointNodeIntersection>
        {
          typedef BuildState<prim::Segment,SegmentPointNodeIntersection> state_type;
          typedef typename state_type::primitive_type primitive_type;
          typedef typename state_type::primitive_cntr_type primitive_cntr_type;
          typedef typename state_type::node_geometry_type node_geometry_type;
          typedef typename state_type::bounds_type bounds_type;
          typedef typename state_type::scalar_type scalar_type;
        };

        struct SegmentSplitPos
        {
          typedef tomo::geometry::prim::Segment Segment;
          typedef Segment::scalar_type scalar_type;

          scalar_type operator()(const Segment* _segment, base::Axis _axis)
          {
            return _segment->p0()[_axis];
          }
        };

        struct SegmentSplitCost
        {
          typedef prim::Segment segment_type;
          typedef segment_type::scalar_type scalar_type;
          scalar_type operator()(const segment_type* _v)
          {
            return 0.25;
          }
        };

        struct SegmentPointTreeBuildPolicy :
            BuildPolicy<SegmentPointBuildState,SegmentSplitPos,SegmentSplitCost>
        {
        
        };

        struct SegmentPointTreeNodeContainer :
            kd::StaticContainer< SegmentPointBuildState >
        {
        };


        /**@brief A SegmentPointTree sorts segments by their first point
          *@detail Required for segment connecting
         **/
        struct SegmentPointTree : Tree<
            SegmentPointTreeNodeContainer,
            SegmentPointTreeBuildPolicy
            >
        {
          typedef SegmentPointTreeBuildPolicy build_policy_type;
          typedef typename build_policy_type::state_type state_type;
          typedef typename state_type::input_type input_type;
          typedef typename state_type::bounds_type bounds_type;

          SegmentPointTree() {}
          SegmentPointTree(const input_type& _input) : Tree(_input) {}
        };
      }
    }
  }
}
