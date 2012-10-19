#pragma once

#include "KDNodeIntersector.hpp"
#include "tomo/geometry/prim/ConnectableSegment.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      namespace intersect
      {
        struct KDNodeVsConnectableSegment : KDNodeIntersector<ConnectableSegment>
        {
          aux::KDNodeIntersectResult operator()(primitive_type& _lineSegment,
                                                node_type& _node)
          {
            return aux::KDNodeIntersectResult(_lineSegment[1][_node.axis()] <= _node.splitPos(),
                                              _lineSegment[1][_node.axis()] >= _node.splitPos());
          }
        };
      }
    }
  }
}
