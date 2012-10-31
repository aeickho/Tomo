#pragma once

#include "PointPoint.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace distance
    {
      template<class VERTEX>
      struct VertexVertex
      {
        typedef VERTEX Vertex;
        typedef typename Vertex::scalar_type scalar_type;
        typedef typename Vertex::point_type point_type;

        scalar_type operator()(const Vertex& _a, const Vertex& _b)
        {
          return PointPoint<typename VERTEX::point_type>()(_a.v(),_b.v());
        }
      };
    }
  }
}
