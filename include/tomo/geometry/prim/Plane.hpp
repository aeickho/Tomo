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
    Plane(const point_type& _center, const vector_type& _normal)
      : center_(_center), normal_(_normal)
    {}

    bool intersect(ray_type& _ray, 
                   scalar_type& _tNear, 
                   scalar_type& _tFar, 
                   vector_type* _normal = NULL) const;

    bounds_type bounds() const;

    point_type center_;
    vector_type normal_;
  };
}}}
