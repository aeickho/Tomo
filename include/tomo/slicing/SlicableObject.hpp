#pragma once

#include "SliceStack.hpp"
#include "tomo/geometry/prim/Primitive.hpp"

namespace tomo
{
  namespace slicing
  {
    struct SlicableObject : geometry::prim::Primitive3f
    {
      virtual void slice() = 0;

      TBD_PROPERTY_REF(SliceStack,slices);
    };
  }
}
