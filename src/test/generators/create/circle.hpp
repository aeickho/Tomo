#pragma once

#include <boost/geometry/algorithms/correct.hpp>
#include "tomo/geometry/prim/Ring.hpp"

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      namespace create
      {
        /// create demo circle
        geometry::prim::Ring circle(
            geometry::prim::Ring::point_type _center, 
            float _radius,
            bool _inverse = false,
            int _numSegments = 48)
        {

          typedef geometry::prim::Ring Ring;
          typedef Ring::point_type point_type;
          Ring _ring;

          for (int i = 0; i <= _numSegments; i++)
          {
            float _angle = float(i)/float(_numSegments)*M_PI*2.0;
            float _sin = _radius*sin(_angle),
                  _cos = _radius*cos(_angle);

            if (_inverse) _sin = -_sin;
            _ring.push_back(point_type(_cos,_sin) + _center);
          }
          boost::geometry::correct(_ring);
          return _ring;
        }
      }
    }
  }
}

