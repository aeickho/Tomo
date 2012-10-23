#include "tomo/slicing/SegmentStack.hpp"
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <tbd/log.h>

using namespace std;
using namespace tomo::geometry::prim;

namespace tomo
{
  namespace slicing
  {
    SegmentStack::SegmentStack(Slices& _slices) : slices_(_slices)
    {
      initializePlanes();
    }

    void SegmentStack::initializePlanes()
    {
      vector< Slice* > _allSlices = slices_.fetch();
      BOOST_FOREACH( Slice* _slice, _allSlices )
      {
        planes_.insert(make_pair<>(_slice->pos(),SegmentPlane()));
      }
    }

    void SegmentStack::aggregate(const SegmentStack& _segmentStack)
    {
      vector< SegmentPlane* > _thisPlanes = fetch();
      vector<const SegmentPlane* > _otherPlanes = _segmentStack.fetch();

      if (_thisPlanes.size() != _otherPlanes.size())
      {
        LOG_ERR << "Number of planes must be equal!";
        return;
      }

      for (size_t i = 0; i < _thisPlanes.size(); i++)
        _thisPlanes[i]->aggregate(*_otherPlanes[i]);
    }

    void SegmentStack::produceSlices()
    {

    }
  }
}
