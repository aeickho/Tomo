#include "tomo/geometry/prim/Box.hpp"

using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      using slicing::Slices;
      using slicing::Slice;

      void Box::slice() 
      {
        vector<Slice*> _slicesInRange = slices().fetch(min().z(),max().z());

        BOOST_FOREACH(Slice* _slice, _slicesInRange)
          generateSlice(_slice);
      }

      void Box::generateSlice(slicing::Slice* _slice) const
      {
        Polygon _polygon;
        _polygon().outer().reserve(4);
        _polygon().outer().push_back(PointXYf(min().x(),min().y()));
        _polygon().outer().push_back(PointXYf(max().x(),min().y()));
        _polygon().outer().push_back(PointXYf(max().x(),max().y()));
        _polygon().outer().push_back(PointXYf(min().x(),max().y()));
        _slice->add(_polygon);
      }
    }
  }
}
