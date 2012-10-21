#pragma once
#include "NodeGeometry.hpp"
#include "NodeIntersectResult.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      template <class PRIMITIVE>
      NodeIntersectResult intersect(PRIMITIVE& _primitive, NodeGeometry<PRIMITIVE>& _node)
      {
        return NodeIntersectResult();
      }
    }
  }
}

