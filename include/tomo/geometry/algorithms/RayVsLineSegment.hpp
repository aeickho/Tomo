#pragma once

#include <tomo/geometry/prim/LineSegment.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      namespace intersect
      {
        struct RayVsLineSegment : RayIntersector<LineSegment>
        {
          bool operator()(primitive_type& _lineSegment, ray_type& _ray)
          {
            /// The idea is to consider ray as line segment
            vec_type _r = _lineSegment[1] - _lineSegment[0];
            point_type _q = _ray.org();
            vec_type _s = _ray.dir();

            scalar_type _denominator = _r.y() * _s.x() - _s.y() * _r.x();

            if (_denominator == 0.0) return false;

            scalar_type _invDenominator = -1.0 / _denominator;

            vec_type _d = _q - _lineSegment[0];

            scalar_type _t = (_d.x() * _r.y() - _d.y() * _r.x()) * _invDenominator;
            scalar_type _u = (_d.x() * _s.y() - _d.y() * _s.x()) * _invDenominator;

            if (_u < 0.0 || _u > 1.0) return false;

            if (_normal)
            {
              (*_normal)(-_r.y(),_r.x());
            }

            return _ray.intersection(Primitive2f::pointer(),_t,_tNear,_tFar);
          }

          TBD_PROPERTY(vec_type*,normal);
          TBD_PROPERTY(scalar_type,tNear);
          TBD_PROPERTY(scalar_type,tFar);
        };
      }
    }
  }
}
