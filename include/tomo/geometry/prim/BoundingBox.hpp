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
      struct BoundingBox : Primitive3f
      {
        TOMO_PRIMITIVE_TYPES(Primitive3f);

        BoundingBox() : bounds_(bounds_type()) {}
        BoundingBox(point_type _min, point_type _max) : 
          bounds_(bounds_type(_min,_max)) 
        {
        }

        TBD_PROPERTY_REF(bounds_type,bounds);
      };
    }
  }
}
