#pragma once

#include "Slice.hpp"

#include "tomo/geometry/comp/Compound.hpp"
#include "tomo/geometry/prim/Segment.hpp"

namespace tomo
{
  namespace slicing
  {
    /// A SegmentStack holds several planes which are Compounds of Segments 
    struct SegmentStack : 
      geometry::comp::PlaneStack<
      geometry::DEFAULT_TYPE, 
      geometry::comp::Compound<geometry::prim::Segment> >
    {
      typedef geometry::comp::Compound<geometry::prim::Segment> SegmentPlane;

      SegmentStack(Slices& _slices);
      void produceSlices();

      void aggregate(const SegmentStack& _segmentStack);

    private:
      void initializePlanes();

      Slices& slices_;
    };
  }
}
