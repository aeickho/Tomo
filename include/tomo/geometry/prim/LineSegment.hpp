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

        bool intersect(ray_type& _ray, scalar_type& _tNear, scalar_type& _tFar, vector_type* _normal = NULL) const;

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
