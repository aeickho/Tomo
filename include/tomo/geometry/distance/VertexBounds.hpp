#pragma once
#include "PointBounds.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace distance
    {
      template<class VERTEX>
      struct VertexBounds
      {
        typedef VERTEX Vertex;
        typedef typename Vertex::scalar_type scalar_type;
        typedef typename Vertex::bounds_type bounds_type;
        typedef typename Vertex::point_type point_type;

        scalar_type operator()(const Vertex& _v, const bounds_type& _b)
        {
          return PointBounds<point_type,bounds_type>(_v.v(),_b);
        }
      };
    }
  }
}
