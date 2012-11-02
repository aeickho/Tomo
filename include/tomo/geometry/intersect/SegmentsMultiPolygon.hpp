#include "tomo/geometry/kd/Tree.hpp"
#include "tomo/geometry/prim/MultiPolygon.hpp"
#include "tomo/geometry/prim/Segment.hpp"
#include "tomo/geometry/kd/visitor/SegmentMarkers.hpp"
#include "tomo/geometry/kd/build/SegmentSplitCost.hpp"
#include "detail/SegmentsFromMarkers.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace intersect
    {
      /// Determines the intersection set between a multipolygon and a set of segments
      template
        <
          template<class,class> class RESULT_CONTAINER=std::vector
        >
      struct SegmentsMultiPolygon
      {
        typedef prim::Segment Segment; 
        typedef prim::MultiPolygon MultiPolygon;
        typedef RESULT_CONTAINER<Segment,std::allocator<Segment>> Segments;

        void operator()(const Segments& _segments, 
                        const MultiPolygon& _multiPolygon,
                        Segments& _outputSegments)
        {
          typedef kd::Tree<Segment> KDTree;
          typedef kd::build::SegmentSplitCost BuildPolicy;
          typedef kd::visitor::SegmentMarkers Visitor;
          KDTree _kdTree;
          Visitor _visitor(_kdTree);

          Segments _polygonSegments;
          _multiPolygon.fetchSegments(_polygonSegments);
          _kdTree.build<BuildPolicy>(_polygonSegments);

          BOOST_FOREACH( const Segment& _segment, _segments)
          {
            _visitor.segment(_segment);
            _kdTree.traversal(_visitor);
            detail::SegmentsFromMarkers<>()(_segment,_visitor.segmentMarkers(),_outputSegments);
          }
        }
      };
    }
  }
}
