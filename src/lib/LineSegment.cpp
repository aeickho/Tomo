#include "tomo/geometry/prim/LineSegment.hpp"

#include <tbd/log.h>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      LineSegment::LineSegment(point_type _front, point_type _back) :
        front_(_front),
        back_(_back),
        next_(NULL),
        prev_(NULL)
      {
      }

      bool LineSegment::intersect(ray_type& _ray, scalar_type& _tNear, scalar_type& _tFar, vec_type* _normal) const
      {
        /// The idea is to consider line segment as ray 
        
        point_type _p = front();
        vec_type _r = back() - front();
        point_type _q = _ray.org();
        vec_type _s = _ray.dir();

        scalar_type _denominator = _r.y() * _s.x() - _s.y() * _r.x();

          if (_denominator == 0.0) return false;

          scalar_type _invDenominator = -1.0 / _denominator;
          
          vec_type _d = _q - _p;
          
          scalar_type _t = (_d.x() * _r.y() - _d.y() * _r.x()) * _invDenominator;
          scalar_type _u = (_d.x() * _s.y() - _d.y() * _s.x()) * _invDenominator;

          if (_u < 0.0 || _u > 1.0) return false;

          if (_normal)
          {
            (*_normal)(-_r.y(),_r.x());
          }

          return _ray.intersection(Primitive2f::pointer(),_t,_tNear,_tFar);
      }

      LineSegment::vec_type LineSegment::normal() const
      {
        vec_type _d = front() - back();
        return vec_type(-_d.y(),_d.x());
      }

      LineSegment::bounds_type LineSegment::bounds() const
      {
        bounds_type _bounds;
        _bounds.extend(back_);
        return _bounds;
      }

      SplitPlaneIntersect LineSegment::intersect(base::Axis _axis, float _splitPos,
          const bounds_type& _boundsLeft, const bounds_type& _boundsRight) const
      {
        float _p = front()[_axis];
        return SplitPlaneIntersect(_p <= _splitPos,
                                   _p >= _splitPos);
      }

    }
  }
}
