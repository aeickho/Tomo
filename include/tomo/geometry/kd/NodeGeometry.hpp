#pragma once

#include "NodeIntersectResult.hpp"
#include "tomo/geometry/base/Coord.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      template <typename PRIMITIVE>
      struct NodeGeometry
      {
        typedef base::Axis axis_type;
        typedef typename PRIMITIVE::bounds_type bounds_type;
        typedef typename PRIMITIVE::scalar_type scalar_type;

        NodeGeometry(bounds_type& _bounds, axis_type _axis, scalar_type _splitPos) :
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