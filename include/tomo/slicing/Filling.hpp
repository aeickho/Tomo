#pragma once

#include "Procedure.hpp"
#include "LineAction.hpp"
#include <list>

namespace tomo
{
  namespace slicing
  {
    template<class STATE>
    class Filling : Procedure< LineAction<STATE> >
    {
    public:
      typedef LineAction<STATE> LineAction;
      typedef ActionGroup<LineAction> ActionGroup;

      ActionGroup operator()( const geometry::prim::Polygon& _in, 
                              std::list<geometry::prim::Polygon>& _out, 
                              typename LineAction::State& _state )
      {

      }
    };
  }
}


