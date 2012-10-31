#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace distance
    {
      template<class POINT, class BOUNDS>
      struct PointBounds
      {
        typedef POINT Point;
        typedef BOUNDS Bounds;
        typedef typename Point::scalar_type scalar_type;

        scalar_type operator()(const Point& _p, const Bounds& _b)
        {
          if (_b.inside(_p)) return 0;

          scalar_type _minDist = INF;
          TOMO_FOREACH_DIM_(_p,i)
          {
            scalar_type _d = (_p[i] < _b.min()[i]) ? _b.min()[i] : _b.max()[i];
            _d -= _p[i];
            _d *= _d;
            _minDist = std::min(_d,_minDist);
          }
          return _minDist*_minDist;
        }
      };
    }
  }
}
