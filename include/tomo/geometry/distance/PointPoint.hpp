#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace distance
    {
      template<class POINT>
      struct PointPoint
      {
        typedef POINT Point;
        typedef typename Point::scalar_type scalar_type;
        scalar_type operator()(const Point& _a, const Point& _b)
        {
          return (_a - _b).sqrLength();
        } 
      };
    }
  }
}
