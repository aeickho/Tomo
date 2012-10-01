#include "tomo/geometry/prim/Tube.hpp"

#include "math.h"

using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      using slicing::Slices;
      using slicing::Slice;

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
        Polygon _polygon;
        Ring _inner;

        for (int i = 0; i < sides_; i++)
        {
          float _angle = i / float(sides_) * M_PI / 2;
          float _cos = cos(_angle);
          float _sin = sin(_angle);
 
          PointXYf _innerPoint(_cos*innerRadius_,_sin*innerRadius_);
          PointXYf _outerPoint(_cos*outerRadius_,_sin*outerRadius_); 
          _inner.push_back(_innerPoint);
          _polygon().inners().push_back(_inner);
          _polygon().outer().push_back(_outerPoint);
        }

        _slice->add(_polygon);
      }


    }
  }
}
