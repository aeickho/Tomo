#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace intersect
    {
      struct RaySegment
      {
        typedef prim::Segment Segment;
        typedef Segment::scalar_type scalar_type;
        typedef Segment::ray_type ray_type;
        typedef Segment::vec_type vec_type;
        typedef Segment::point_type point_type;

        bool operator()(
          ray_type& _ray,
          const Segment& _segment,
          scalar_type _tNear,
          scalar_type _tFar,
          vec_type* _normal = nullptr)
        {
          /// The idea is to consider ray as line segment
          vec_type _r = _segment[1] - _segment[0];
          point_type _q = _ray.org();
          vec_type _s = _ray.dir();

          scalar_type _denominator = _r.y() * _s.x() - _s.y() * _r.x();

          if (_denominator == 0.0) return false;

          scalar_type _invDenominator = -1.0 / _denominator;
          vec_type _d = _q - _segment[0];
          scalar_type _t = (_d.x() * _r.y() - _d.y() * _r.x()) * _invDenominator;
          scalar_type _u = (_d.x() * _s.y() - _d.y() * _s.x()) * _invDenominator;

          if (_u < 0.0 || _u > 1.0) return false;

          if (_normal)
          {
            (*_normal)(-_r.y(),_r.x());
          }

          return _ray.intersection(_segment.pointer(),_t,_tNear,_tFar);
        }

      };
    }
  }
}
