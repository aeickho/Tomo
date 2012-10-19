#pragma once
#include "tomo/geometry/aux/KDNodeIntersectResult.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace algorithms
    {
      template <class PRIMITIVE>
      aux::KDNodeIntersectResult intersect(PRIMITIVE& _primitive, aux::KDNodeGeometry<PRIMITIVE>& _node)
      {
        return aux::KDNodeIntersectResult();
      }
    }
  }
}

