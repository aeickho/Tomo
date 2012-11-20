#pragma once

#include "tomo/slicing/Procedure.hpp"
#include "tomo/slicing/LineAction.hpp"

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
        typedef geometry::prim::Segment Segment;
        typedef PATTERN pattern_type;

        LineFilling(const pattern_type& _pattern) : pattern_(_pattern) {}

        MultiLineString operator()( const MultiPolygon& _in,
                                          MultiPolygon& _out )
        {
          ActionGroup _actionGroup;
          MultiLineString _fillingPattern,
                          _output;
          pattern_(_in.bounds(),_fillingPattern);
          boost::geometry::intersection(_fillingPattern,_in,_output);
          return _output;
        }

        TBD_PROPERTY_REF(pattern_type,pattern);
      };
    }
  }
}


