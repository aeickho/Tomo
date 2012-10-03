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
      using base::Point2f;

      void Box::slice() 
      {
        vector<Slice*> _slicesInRange = slices().fetch(min().z(),max().z());

        BOOST_FOREACH(Slice* _slice, _slicesInRange)
          generateSlice(_slice);
      }

      void Box::generateSlice(slicing::Slice* _slice) const
      {
        Polygon _polygon;
        Ring _outer(Ring::OUTER);
        _outer.lineSegments().reserve(4);
        _outer.add(Point2f(min().x(),min().y()));
        _outer.add(Point2f(max().x(),min().y()));
        _outer.add(Point2f(max().x(),max().y()));
        _outer.add(Point2f(min().x(),max().y()));
        _polygon.add(_outer);
        _slice->add(_polygon);
      }
    }
  }
}
