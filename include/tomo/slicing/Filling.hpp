#pragma once

#include "Procedure.hpp"

namespace tomo
{
  namespace slicing
  {
    template<class STATE> class Filling : Procedure<LineAction,STATE>
    {
    public:
      typedef GroupAction<LineAction,STATE> GroupAction;

      GroupAction operator()( const geometry::prim::Polygon& _in, 
                              const std::list<geometry::prim::Polygon>& _out, 
                              STATE& _state );
    }

  }
}


