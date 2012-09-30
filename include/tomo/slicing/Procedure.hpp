#pragma once

#include "ActionGroup.hpp"
#include "tomo/geometry/prim/Polygon.hpp"

namespace tomo
{
  namespace slicing
  {
    /// A procedure generates a sequence of actions
    template<class ACTION> class Procedure
    {
    public:
      typedef ActionGroup<ACTION> ActionGroup;

      typedef typename geometry::prim::Polygon Polygon;
      typedef typename geometry::prim::MultiPolygon MultiPolygon;

      /// Polygon = Isle
      ActionGroup operator()( const Polygon& _in,
                              MultiPolygon& _out,
                              typename ACTION::State& _state );
    };
  }
}
