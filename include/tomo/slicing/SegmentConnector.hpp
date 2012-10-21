#pragma once

#include "tomo/geometry/prim/ConnectableSegment.hpp"
#include "tomo/geometry/prim/Ring.hpp"
#include "tomo/geometry/kd/Tree.hpp"

namespace tomo
{
  namespace slicing
  {
    /// A SegmentConnector connects a vector of ConnectableSegments to Rings
    struct SegmentConnector
    {
      typedef geometry::prim::ConnectableSegment linesegment_type;
      typedef geometry::prim::Ring ring_type;
      typedef geometry::kd::Tree<linesegment_type> kdtree_type;

      bool operator()(
          std::vector<linesegment_type>& _inSegments, 
          std::vector<ring_type>& _outRings);
    
    private:
      kdtree_type kdTree_;
    };
  }
}
