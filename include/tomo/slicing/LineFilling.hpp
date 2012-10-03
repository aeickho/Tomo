#pragma once

#include "Procedure.hpp"
#include "LineAction.hpp"
#include <list>

namespace tomo
{
  namespace slicing
  {
    template<class STATE>
    class LineFilling 
    {
    public:
      typedef STATE State;
      typedef LineAction<State> LineAction;
      typedef ActionGroup<LineAction> ActionGroup;
      typedef geometry::prim::Polygon Polygon;
      typedef geometry::base::Vec2f Vec2f;
      typedef geometry::base::Point2f Point2f;
      typedef geometry::aux::Ray2f Ray2f;
      typedef geometry::prim::LineSegment Linesignemt;

      ActionGroup operator()( const Polygon& _in, 
                              std::list<Polygon>& _out, 
                              typename LineAction::State& _state )
      {
        ActionGroup _actionGroup;
        lineSegments_.clear();

        Ray2f _rayBegin, _rayEnd;
        _in.boundingRays(angle_,_rayBegin,_rayEnd);
        
        Vec2f _fillingDirection = _rayEnd.org() - _rayBegin.org();

        unsigned _nLines = _fillingDirection.length() / gap_;   
        Vec2f _rayInc = _fillingDirection * (1.0 / _nLines);
        
        Ray2f _ray(_rayBegin.org() + 0.5*_rayInc,_rayBegin.dir(),0.0,1.0);

        for (unsigned i = 1; i < _nLines; ++i)
        {
          _in.lineSegments(_ray,lineSegments_);
          _ray.org() += _rayInc;
        }

        //_actionGroup.generate(_lineSegments,_state);
        return _actionGroup;
      }

      std::vector<geometry::prim::LineSegment> lineSegments_;

      TBD_PROPERTY(float,gap);
      TBD_PROPERTY(float,angle);
    };
  }
}


