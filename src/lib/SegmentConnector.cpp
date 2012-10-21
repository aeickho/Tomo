#include "tomo/slicing/SegmentConnector.hpp"

#include "tomo/geometry/kd/NodeVsConnectableSegment.hpp"

using namespace std;

namespace tomo
{
  namespace slicing
  {
    bool SegmentConnector::operator()(vector<linesegment_type>& _inSegments,
                                      vector<ring_type>& _outRings)
    {
      kdTree_.build(_inSegments,3);
      ///@todo Implement connecting algorithm

      return true;
    }
  }
}
