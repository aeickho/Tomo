#pragma once

#include "Primitive.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      struct Plane : Primitive3f
      {
        Plane(const point_type& _center, const vec_type& _normal)
          : center_(_center), normal_(_normal)
        {}

        bounds_type bounds() const
        {
          return Plane::bounds_type(point_type(-INF,-INF,-INF),point_type(INF,INF,INF));          
        }

        point_type center_;
        vec_type normal_;
      };
    }
  }
}
