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

        bounds_type bounds() const
        {      
          point_type _rP(radius_,radius_,radius_),
                     _rM(-radius_,-radius_,-radius_);
          return bounds_type(center_ + _rM, center_ + _rP);
        }

        TBD_PROPERTY(point_type,center);
        TBD_PROPERTY(scalar_type,radius);
      };
    }
  }
}
