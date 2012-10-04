#include "tomo/geometry/prim/Primitive.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      struct Sphere : public Primitive3f
      {
        Sphere(const point_type& _center, scalar_type _radius)
          : center_(_center), radius_(_radius)
        {}

        bool intersect(ray_type& _ray, scalar_type& _tNear, scalar_type& _tFar, vec_type* _normal = NULL) const;

        bounds_type bounds() const;

        TBD_PROPERTY(point_type,center);
        TBD_PROPERTY(scalar_type,radius);
      };
    }
  }
}
