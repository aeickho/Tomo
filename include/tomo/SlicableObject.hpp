#pragma once

#include "tomo/Primitive.hpp"
#include "tomo/Slice.hpp"

namespace tomo 
{
  struct SlicableObject : public Primitive3f
  {
    virtual bool slice(Slice& _slice) const = 0;

  };
}
