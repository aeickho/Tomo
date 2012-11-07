#pragma once

#include "LineAction.hpp"
#include <list>

namespace tomo
{
  namespace slicing
  {
    /// concept: Procedure 
    template<class STATE>
    class Perimeter 
    {
    public:
      typedef STATE State;
      typedef LineAction<State> LineAction;
      typedef ActionGroup<LineAction> ActionGroup;
      typedef geometry::prim::Polygon Polygon;
      typedef geometry::base::Vec2f Vec2f;
      typedef geometry::base::Point2f Point2f;
      typedef geometry::aux::Ray2f Ray2f;

      ActionGroup operator()( const Polygon& _in, 
                              std::list<Polygon>& _out, 
                              typename LineAction::State& _state )
      {
        ActionGroup _actionGroup;

//        _out.push_back(_in.shrinked(borderWidth_));
        _out.push_back(_in);
        return _actionGroup;
      }

      TBD_PROPERTY(float,borderWidth);
    };
  }
}


