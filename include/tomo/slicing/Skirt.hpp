#pragma once

#include "tomo/slicing/SliceStack.hpp"
#include "tomo/geometry/algorithm/RingOffset.hpp"

namespace tomo
{
  namespace slicing
  {
    struct Skirt
    {
      typedef geometry::prim::Ring Ring;
      typedef Ring::scalar_type scalar_type;
      
      Ring operator()(scalar_type _width, const SliceStack& _sliceStack)
      {
        using geometry::algorithm::RingOffset;
        using geometry::prim::MultiPolygon;
        using geometry::prim::Polygon;
        Ring _skirt;

        std::vector<const Slice*> _slices = _sliceStack.fetch();

        for (const Slice* _slice : _slices)
        {
          Ring _hull;
          boost::geometry::convex_hull(_slice->polygons(),_hull);

          MultiPolygon _unionOutput;
          boost::geometry::union_(Polygon(_hull),Polygon(_skirt),_unionOutput);
          boost::geometry::convex_hull(_unionOutput,_skirt);
        }

        boost::geometry::correct(_skirt);

        RingOffset<>()(_skirt,_width);
        return _skirt;
      }
    };
  }
}
