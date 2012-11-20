#include "tomo/geometry/prim/Tube.hpp"

#include <boost/foreach.hpp>
#include <math.h>

using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      using slicing::SliceStack;
      using slicing::Slice;
      using base::Point2f;

      Tube::Tube(scalar_type _outerRadius, 
                 scalar_type _innerRadius,
                 scalar_type _height,
                 int _sides) : 
          outerRadius_(_outerRadius), 
          innerRadius_(_innerRadius), 
          height_(_height),
          sides_(_sides) 
      {}

      void Tube::slice() 
      {
        vector<Slice*> _slicesInRange = slices().fetch(0.0,height_);

        BOOST_FOREACH(Slice* _slice, _slicesInRange)
          generateSlice(_slice);
      }

      void Tube::generateSlice(slicing::Slice* _slice) const
      {
        Ring _outer(Ring::OUTER);
        Ring _inner(Ring::INNER);
        Polygon _polygon;

        for (int i = 0; i < sides_; i++)
        {
          float _angle = i / float(sides_) * M_PI / 2;
          float _cos = cos(_angle);
          float _sin = sin(_angle);
 
          Point2f _innerPoint(_cos*innerRadius_,_sin*innerRadius_);
          Point2f _outerPoint(_cos*outerRadius_,_sin*outerRadius_);  
          _inner.push_back(_innerPoint);
          _outer.push_back(_outerPoint);
        }

        _polygon.add(_outer);
        _polygon.add(_inner);
        _slice->add(_polygon);
      }
    }
  }
}
