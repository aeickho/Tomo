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

      Skirt(scalar_type _width = 0.05) : width_(_width) {}

      Ring operator()(const SliceStack& _sliceStack)
      {
        using geometry::algorithm::RingOffset;
        using geometry::prim::MultiPolygon;
        using geometry::prim::Polygon;
        Ring _skirt;

        std::vector<const Slice*> _slices = _sliceStack.fetch();

        for (const Slice* _slice : _slices)
        {
          /// Build convex hull for each slice
          Ring _hull;
          boost::geometry::convex_hull(_slice->polygons(),_hull);

          /// Unify it with hull of preceding slices 
          MultiPolygon _unionOutput;
          boost::geometry::union_(Polygon(_hull),Polygon(_skirt),_unionOutput);
          boost::geometry::convex_hull(_unionOutput,_skirt);
        }

        _skirt.update();

        /// Offset skirt with a certain width
        RingOffset<>()(_skirt,width_);
        return _skirt;
      }

      TBD_PROPERTY(scalar_type,width)
    };
  }
}
