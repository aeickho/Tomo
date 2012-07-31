#pragma once

#include "tomo/Primitive.hpp"
#include "tomo/Slice.hpp"

namespace tomo 
{
  struct SlicableObject : public Primitive3f
  {
    virtual void slice(Slices& _slices) const = 0;

  };
}
