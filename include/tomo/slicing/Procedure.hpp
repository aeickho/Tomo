#pragma once

#include "ACTIONGroup.hpp"
#include "tomo/geometry/prim/Polygon"

namespace tomo
{
  namespace slicing
  {
    /// A procedure generates a sequence of actions
    template<class ACTION, class STATE> class Procedure
    {
    public:
      typedef ACTION<STATE> ACTION;
      using geometry::prim::Polygon;
 
      /// Polygon = Isle
      GroupAction<ACTION,STATE> operator()( const geometry::prim::Polygon& _in, 
                                            const std::list<geometry::prim::Polygon>& _out, 
                                            STATE& _state );
    };
  }
}
