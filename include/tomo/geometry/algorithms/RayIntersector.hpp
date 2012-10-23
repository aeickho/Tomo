#pragma once

#include <tomo/geometry/aux/Ray.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace algorithms
    {
      template <class PRIMITIVE>
      bool intersect(
          const PRIMITIVE& _primitive, 
          typename PRIMITIVE::ray_type& _ray, 
          typename PRIMITIVE::scalar_type* _tNear = nullptr,
          typename PRIMITIVE::scalar_type* _tFar = nullptr)
      {

      }
    }
  }
}
