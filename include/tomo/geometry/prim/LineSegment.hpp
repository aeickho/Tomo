#pragma once

#include "Primitive.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      struct LineSegment : public Primitive2f
      {
        LineSegment(point_type _front, point_type _back);

        bool intersect(ray_type& _ray, scalar_type& _tNear, scalar_type& _tFar, vector_type* _normal = NULL) const
        {
          vector_type _dLine = front() - back();
          scalar_type _denominator = _dLine.y() * _ray.dir().x() - _dLine.x() * _ray.dir().y();
          if (_denominator == 0.0) return false;

          scalar_type _invDenominator = 1.0 / _denominator;
          vector_type _dRayLine = _ray.org() - back();
          scalar_type _t = (_dLine.x() * _dRayLine.y() - _dLine.y() * _dRayLine.x()) * _invDenominator;
          scalar_type _s = (_dLine.y() * _dRayLine.x() - _dLine.x() * _dRayLine.y()) * _invDenominator;

          if (_s < 0.0 || _s > 1.0) return false;
          if (_normal)
          {
            (*_normal)(-_dLine.y(),_dLine.x());
          }

          return _ray.intersection(Primitive2f::pointer(),_t,_tNear,_tFar);
        }

        vector_type distanceVec(const Primitive& _p) const
        {
          return _p.center() - front();
        }

        bounds_type bounds() const;
        SplitPlaneIntersect intersect(base::Axis _axis, scalar_type _splitPos,
                                      const bounds_type& _boundsLeft, const bounds_type& _boundsRight) const;

        TBD_PROPERTY_REF(point_type,front);
        TBD_PROPERTY_REF(point_type,back);
        TBD_PROPERTY(LineSegment*,next);
        TBD_PROPERTY(LineSegment*,prev);
      };
    }
  }
}
