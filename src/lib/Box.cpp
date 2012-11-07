#include "tomo/geometry/prim/Box.hpp"

#include <boost/foreach.hpp>

using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      using slicing::Slice;
      using base::Point2f;

      void Box::slice() 
      {
        vector<Slice*> _slicesInRange = 
          slices().fetch(bounds().min().z(),bounds().max().z());

        BOOST_FOREACH(Slice* _slice, _slicesInRange)
          generateSlice(_slice);
      }

      void Box::generateSlice(slicing::Slice* _slice) const
      {
        Polygon _polygon;
        Ring _outer(Ring::OUTER);


        const point_type& _min = bounds().min();
        const point_type& _max = bounds().max();

        typedef Ring::point_type ring_point_type;
        
        _outer.reserve(4);
        _outer.add(ring_point_type(_min.x(),_min.y()));
        _outer.add(ring_point_type(_max.x(),_min.y()));
        _outer.add(ring_point_type(_max.x(),_max.y()));
        _outer.add(ring_point_type(_min.x(),_max.y()));
        _polygon.add(_outer);
        _slice->add(_polygon);
      }
    }
  }
}
