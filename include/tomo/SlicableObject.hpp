#pragma once

#include "tomo/Primitive.hpp"
#include "tomo/Slice.hpp"

namespace tomo 
{
  struct SlicableObject : public Primitive
  {
    virtual bool slice(Slice& _slice) const = 0;

  };
}
