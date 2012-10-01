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
      typedef STATE State;
      typedef LineAction<State> LineAction;
      typedef ActionGroup<LineAction> ActionGroup;
      typedef typename geometry::prim::Polygon Polygon;
      typedef typename geometry::base::Vec2f Vec2f;
      typedef typename geometry::aux::Ray2f Ray2f;

      ActionGroup operator()( const Polygon& _in, 
                              std::list<Polygon>& _out, 
                              typename LineAction::State& _state )
      {
        ActionGroup _actionGroup;

        lineSegments_.clear();
        geometry::aux::Ray2f _ray(_in.bounds().min().vec(),Vec2f(_in.bounds().size().x(),0),0.0,1.0); 
        Vec2f _rayInc(0.0,gap_);

        while (_ray.org().y() < _in.bounds().max().y())
        {
          _ray.org() += _rayInc;
          _in.lineSegments(_ray,lineSegments_);
        }

        //_actionGroup.generate(_lineSegments,_state);
        return _actionGroup;
      }

      std::vector<geometry::prim::LineSegment> lineSegments_;

      TBD_PROPERTY(float,gap);
    };
  }
}


