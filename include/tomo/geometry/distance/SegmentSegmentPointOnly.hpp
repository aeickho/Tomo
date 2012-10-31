#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace distance
    {
      struct SegmentSegmentPointOnly
      {
        typedef geometry::prim::Segment Segment;
        typedef Segment::scalar_type scalar_type;

        scalar_type operator()(const Segment& _a, const Segment& _b)
        {
          return (_a[1] - _b[0]).sqrLength(); 
        }
      };
    }
  }
}
