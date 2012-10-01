#pragma once

#include "Procedure.hpp"
#include "LineAction.hpp"
#include <list>

namespace tomo
{
  namespace slicing
  {
    template<class STATE>
    class Perimeter : Procedure< LineAction<STATE> >
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

        return _actionGroup;
      }

      std::vector<geometry::prim::LineSegment> lineSegments_;

      TBD_PROPERTY(float,gap);
      TBD_PROPERTY(float,angle);
    };
  }
}


