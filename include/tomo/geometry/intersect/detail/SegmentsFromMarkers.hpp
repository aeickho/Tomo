#pragma once

#include "tomo/geometry/prim/Segment.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace intersect
    {
      namespace detail
      {
        template
        <
          template<class,class> class SEG_CONTAINER = std::vector,
          typename SEGMENT = tomo::geometry::prim::Segment,
          typename SEGMENT_MARKER = typename SEGMENT::scalar_type
        >
        struct SegmentsFromMarkers
        {
          typedef SEGMENT Segment;
          typedef typename Segment::scalar_type scalar_type;
          typedef typename Segment::point_type point_type;
          typedef SEGMENT_MARKER SegmentMarker;
          typedef std::set<SegmentMarker> SegmentMarkers;
          typedef SEG_CONTAINER<Segment,std::allocator<Segment>> Segments;

          void operator()(const Segment& _segment,
                          const SegmentMarkers& _segMarkers,
                          Segments& _segments)
          {
            int i = 0;
            _segments.reserve(_segments.size() + _segMarkers.size()/2);
            point_type _points[2];
            BOOST_FOREACH( const SEGMENT_MARKER& _segMarker, _segMarkers )
            {
              _points[i % 2] = _segment[0] + _segMarker * (_segment[1] - _segment[0]);
              if (i % 2 == 1)
              {
                _segments.push_back(SEGMENT(_points[0],_points[1]));
              }
              i++;
            }
          }
        };
      }
    }
  }
}
