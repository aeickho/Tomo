#include "tomo/slicing/SegmentConnector.hpp"

#include "tomo/geometry/prim/intersect/KDNodeVsConnectableSegment.hpp"

using namespace std;

namespace tomo
{
  namespace slicing
  {
    bool SegmentConnector::operator()(vector<linesegment_type>& _inSegments,
                                      vector<ring_type>& _outRings)
    {
      typedef geometry::prim::intersect::KDNodeVsConnectableSegment NodeIntersector;
      kdTree_.build<NodeIntersector>(_inSegments,3);

      return true;
    }
  }
}
