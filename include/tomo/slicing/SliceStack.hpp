#pragma once

#include "tomo/geometry/comp/PlaneStack.hpp"
#include "Slice.hpp"

namespace tomo
{
  namespace slicing
  {
    struct SliceStack : geometry::comp::PlaneStack<Slice>
    {
    public:
      SliceStack() {}
      SliceStack(float _thickness, const bounds_type& _bounds);

      void build(float _thickness, const bounds_type& _bounds);
    };
  }
}
