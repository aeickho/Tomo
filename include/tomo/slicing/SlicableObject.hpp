#pragma once

#include "tomo/slicing/Slice.hpp"
#include "tomo/geometry/prim/Primitive.hpp"

namespace tomo
{
  namespace slicing
  {
    struct SlicableObject : public geometry::prim::Primitive3f
    {
      virtual void slice() = 0;

      TBD_PROPERTY_REF(geometry::base::Vec3f,pos);
      TBD_PROPERTY_REF(Slices,slices);
    };
  }
}
