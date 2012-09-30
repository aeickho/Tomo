#pragma once

#include "Action.hpp"

namespace tomo
{
  namespace slicing
  {
    template<class STATE> class LineAction : public Action<STATE>
    {
    public:
      typedef Action<STATE> Action;
  
      LineAction( Point3f _begin, Point3f _end ) :
        begin_(_begin),
        end_(_end)
      {
      }

      virtual void process( const STATE& _state )
      {
        _state.pos(pos_);
      }

      TBD_PROPERTY(Point3f,begin);
      TBD_PROPERTY(Point3f,end);
    };
  }
}
