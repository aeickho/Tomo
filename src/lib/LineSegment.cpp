#include "tomo/geometry/prim/LineSegment.hpp"

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
