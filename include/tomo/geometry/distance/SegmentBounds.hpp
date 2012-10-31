#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace distance
    {
      struct SegmentBounds
      {
        typedef prim::Segment Segment;
        typedef Segment::scalar_type scalar_type;
        typedef Segment::point_type point_type;
        typedef Segment::bounds_type bounds_type;

        scalar_type operator()(const Segment& _s, const bounds_type& _b)
        {
          const point_type& _p = _s[1];
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

