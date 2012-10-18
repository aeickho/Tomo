#pragma once

#include "RayIntersector.hpp"
#include <tomo/geometry/prim/Plane.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      namespace intersect
      {
        struct RayVsPlane : RayIntersector<Plane>
        {
          bool operator()(Plane& _plane, ray_type& _ray) const
          {
            scalar_type dn = dot(_ray.dir(),_plane.normal());
            if (dn == 0.0f) return false;

            vec_type org = _ray.org() - _plane.center();

            scalar_type d = -dot(_plane.normal(),org) / dn;
            if (d < 0) return false;

            vec_type iPoint = _ray.org() +_ray.dir() *d - _plane.center();

            if (normal_) (*normal_)(_plane.normal());

            return _ray.intersection(this->pointer(),d,_tNear,_tFar);
          }

          TBD_PROPERTY(vec_type*,normal);
          TBD_PROPERTY(scalar_type,tNear);
          TBD_PROPERTY(scalar_type,tFar);
        };
      }
    }
  }
