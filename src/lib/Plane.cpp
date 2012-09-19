#include "tomo/geometry/prim/Plane.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      bool Plane::intersect(ray_type& _ray, scalar_type& _tNear, scalar_type&_tFar, vector_type* _normal) const
      {
        scalar_type dn = dot(_ray.dir(),normal_);
        if (dn == 0.0f) return false;

        vector_type org = _ray.org() - center_;

        scalar_type d = -dot(normal_,org) / dn;
        if (d < 0) return false;

        vector_type iPoint = _ray.org() +_ray.dir() *d - center_;

        if (_normal) (*_normal)(normal_);

        return _ray.intersection(this->pointer(),d,_tNear,_tFar);
      }

      Plane::bounds_type Plane::bounds() const
      {
        return Plane::bounds_type(point_type(-INF,-INF,-INF),point_type(INF,INF,INF));
      }
    }
  }
}
