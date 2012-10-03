#pragma once

#include "Action.hpp"

namespace tomo
{
  namespace slicing
  {
    template<class STATE> struct LineAction 
    {
      typedef STATE State;
      typedef Action<State> Action;
      typedef geometry::base::Point3f Point3f;

      LineAction( Point3f _begin, Point3f _end ) :
        begin_(_begin),
        end_(_end)
      {}
      void process( const State& _state )
      {
      }
      
      TBD_PROPERTY(Point3f,begin);
      TBD_PROPERTY(Point3f,end);
    };
  }
}
