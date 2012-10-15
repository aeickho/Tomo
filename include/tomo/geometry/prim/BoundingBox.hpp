#pragma once

#include "Primitive.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      /** @brief A bounding is primitive which defines bounds of a compound object
       */
      struct BoundingBox : public Primitive3f, public aux::Bounds3f
      {
        TOMO_PRIMITIVE_TYPES(Primitive3f);

        using aux::Bounds3f::min;
        using aux::Bounds3f::max;

        BoundingBox() : bounds_type() {}
        BoundingBox(point_type _min, point_type _max) : bounds_type(_min,_max) {}

        bool intersect(ray_type& _ray,
                       scalar_type& _tNear,
                       scalar_type& _tFar,
                       vector_type* _normal = NULL) const;

        bounds_type bounds() const
        {
          return bounds_type(min(),max());
        }
      };
    }
  }
}
