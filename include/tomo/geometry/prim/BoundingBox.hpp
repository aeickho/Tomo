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
        typedef float scalar_type;
        typedef base::Point<3,scalar_type> point_type;
        typedef base::Vec<3,scalar_type> vector_type;

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
