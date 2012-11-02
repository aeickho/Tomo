#pragma once

#include "detail/Pattern.hpp"
#include "tomo/slicing/Procedure.hpp"
#include "tomo/slicing/LineAction.hpp"

#include "tomo/geometry/intersect/SegmentsMultiPolygon.hpp"

namespace tomo
{
  namespace slicing
  {
    /// concept: Procedure
    template<class STATE, class PATTERN>
    class LineFilling 
    {
    public:
      typedef STATE State;
      typedef LineAction<State> LineAction;
      typedef ActionGroup<LineAction> ActionGroup;
      typedef geometry::prim::MultiPolygon MultiPolygon;
      typedef geometry::base::Vec2f Vec2f;
      typedef geometry::base::Point2f Point2f;
      typedef geometry::base::Ray2f Ray2f;
      typedef geometry::prim::Segment Segment;

      ActionGroup operator()( const MultiPolygon& _in, 
                              MultiPolygon& _out, 
                              typename LineAction::State& _state )
      {
        ActionGroup _actionGroup;
        lineSegments_.clear();

        using geometry::intersect::SegmentsMultiPolygon;
        
        std::vector<Segment> _segments;
        PATTERN()(_in.bounds(),_segments);
        SegmentsMultiPolygon<>()(_segments,_in,lineSegments_);

        return _actionGroup;
      }

      std::vector<Segment> lineSegments_;

      TBD_PROPERTY(float,gap);
      TBD_PROPERTY(float,angle);
    };
  }
}


