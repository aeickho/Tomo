#pragma once

#include "tomo/geometry/comp/Compound.hpp"

#include "Slice.hpp"
#include "SliceStack.hpp"

namespace tomo
{
  namespace slicing
  {
    /// A SegmentStack holds several planes which are Compounds of Segments 
    struct SegmentStack : 
      geometry::comp::PlaneStack<geometry::comp::Compound<geometry::prim::Segment>>
    {
      typedef geometry::comp::Compound<geometry::prim::Segment> SegmentPlane;

      SegmentStack(SliceStack& _sliceStack);
    private:
      void initializePlanes();

      SliceStack& slices_;
    };
  }
}
