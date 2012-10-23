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
      NodeIntersectResult intersect(const PRIMITIVE& _primitive, 
                                    const NodeGeometry<typename PRIMITIVE::model_type>& _node)
      {
        return NodeIntersectResult();
      }
    }
  }
}

