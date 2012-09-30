#pragma once

#include "Procedure.hpp"

namespace tomo
{
  namespace slicing
  {
    template<class STATE> class Filling : Procedure<LineAction,STATE>
    {
    public:
      typedef ActionGroup<LineAction,STATE> ActionGroup;

      ActionGroup operator()( const geometry::prim::Polygon& _in, 
                              const std::list<geometry::prim::Polygon>& _out, 
                              STATE& _state );
    }

  }
}


