#pragma once

#include <tomo/slicing/Slice.hpp>
#include <tomo/geometry/kd/Tree.hpp>
#include "tomo/geometry/prim/Vertex.hpp"
#include "tomo/geometry/prim/Polygon.hpp"
#include "tomo/geometry/prim/LineString.hpp"
#include "tomo/slicing/SegmentStack.hpp"

namespace tomo
{
  namespace draw
  {
    template<class MODEL> 
    struct DrawInterface : MODEL
    {
      TOMO_MODEL_TYPES(MODEL);

      typedef geometry::base::Point<MODEL> point_type;
      typedef geometry::base::Vec<MODEL> vec_type;
      typedef geometry::base::Bounds<MODEL> bounds_type;
      typedef geometry::base::Ray<MODEL> ray_type;

    };

  }
}
