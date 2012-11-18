#pragma once

#include "tomo/slicing/Procedure.hpp"
#include "tomo/slicing/LineAction.hpp"

//#include "tomo/geometry/intersect/SegmentsMultiPolygon.hpp"

namespace tomo
{
  namespace slicing
  {
    namespace filling
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
        typedef geometry::prim::MultiLineString MultiLineString;
        typedef geometry::base::Vec2f Vec2f;
        typedef geometry::base::Point2f Point2f;
        typedef geometry::base::Ray2f Ray2f;
        typedef geometry::base::Bounds2f Bounds2f;
        typedef geometry::prim::Segment Segment;

        ActionGroup operator()( const MultiPolygon& _in,
                                MultiPolygon& _out )
        {
          ActionGroup _actionGroup;
          fillingPattern_.clear();
          MultiLineString _fillingPattern;

          ///@todo Bounding is hard-coded for now, implement MultiPolygon::bounds()!!!
          PATTERN()(Bounds2f(Point2f(-0.3,-0.3),Point2f(1.7,1.7)),_fillingPattern);

          boost::geometry::intersection(_fillingPattern,_in,fillingPattern_);

          return _actionGroup;
        }

        MultiLineString fillingPattern_;

        TBD_PROPERTY(float,gap);
        TBD_PROPERTY(float,angle);
      };
    }
  }
}


