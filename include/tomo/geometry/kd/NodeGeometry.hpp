#pragma once

#include "NodeIntersectResult.hpp"
#include "tomo/geometry/base/Coord.hpp"
#include "tomo/geometry/base/Bounds.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      template <typename MODEL>
      struct NodeGeometry : MODEL
      {
        TOMO_MODEL_TYPES(MODEL);
        typedef base::Axis axis_type;
        typedef base::Bounds<MODEL> bounds_type;

        NodeGeometry(const bounds_type& _bounds, 
                     const axis_type _axis, 
                     const scalar_type _splitPos) :
          bounds_(_bounds),
          axis_(_axis),
          splitPos_(_splitPos)
        {}

        TBD_PROPERTY_REF(bounds_type,bounds);
        TBD_PROPERTY(axis_type,axis);
        TBD_PROPERTY(scalar_type,splitPos);
      };
    }
  }
}
