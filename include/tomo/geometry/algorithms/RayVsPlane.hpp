#pragma once

#include "RayIntersector.hpp"
#include <tomo/geometry/prim/Plane.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace algorithms
    {
      bool intersect(
        const Plane& _plane,
        Plane::ray_type& _ray,
        Plane::scalar_type* _tNear = nullptr,
        Plane::scalar_type* _tFar = nullptr)
      {
        scalar_type dn = dot(_ray.dir(),_plane.normal());
        if (dn == 0.0f) return false;

        vec_type org = _ray.org() - _plane.center();

        scalar_type d = -dot(_plane.normal(),org) / dn;
        if (d < 0) return false;

        vec_type iPoint = _ray.org() +_ray.dir() *d - _plane.center();

        if (normal_) (*normal_)(_plane.normal());

        return _ray.intersection(this->pointer(),d,*_tNear,*_tFar);
      }
    }
  }
}
