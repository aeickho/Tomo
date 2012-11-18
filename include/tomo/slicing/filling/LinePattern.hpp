#pragma once

#include "tomo/geometry/prim/LineString.hpp"
#include "tomo/geometry/algorithm/BoundingSegments.hpp"

namespace tomo
{
  namespace slicing
  {
    namespace filling
    {
      /// Generate the line pattern for filling
      struct LinePattern
      {
        typedef geometry::prim::Segment Segment;
        typedef geometry::prim::MultiLineString MultiLineString;
        typedef geometry::prim::LineString LineString;
        typedef typename MultiLineString::point_type point_type;
        typedef typename MultiLineString::vec_type vec_type;
        typedef typename MultiLineString::scalar_type scalar_type;
        typedef typename MultiLineString::bounds_type bounds_type;

        LinePattern(scalar_type _angle = 0.0, scalar_type _gap = 0.01) : 
          angle_(_angle), gap_(_gap) {}

        void operator()(const bounds_type& _bounds,
                        MultiLineString& _pattern)
        {
          using geometry::algorithm::BoundingSegments;
          std::pair<Segment,Segment> _boundingSegments = BoundingSegments<>()(_bounds,angle_);
          vec_type _fillingDirection = _boundingSegments.second[0] - _boundingSegments.first[0];

          unsigned _nLines = _fillingDirection.length() / gap_;
          vec_type _inc = _fillingDirection * (1.0 / _nLines);

          LineString _lineString;
          _lineString.push_back(_boundingSegments.first[0] + 0.5*_inc);
          _lineString.push_back(_boundingSegments.first[1] + 0.5*_inc);

          for (unsigned i = 1; i < _nLines; ++i)
          {
            _pattern.push_back(_lineString);
            _lineString[0] += _inc;
            _lineString[1] += _inc;
            std::swap(_lineString[0],_lineString[1]);
          }
        }

        TBD_PROPERTY(scalar_type,angle);
        TBD_PROPERTY(scalar_type,gap);
      };
    }
  }
}
