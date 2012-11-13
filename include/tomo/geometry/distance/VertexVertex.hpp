#pragma once

#include "PointPoint.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace distance
    {
      template<class MODEL>
      struct VertexVertex
      {
        typedef prim::Vertex<MODEL> Vertex;
        typedef typename Vertex::scalar_type scalar_type;
        typedef typename Vertex::point_type point_type;

        scalar_type operator()(const Vertex& _a, const Vertex& _b)
        {
          return PointPoint<point_type>()(_a.v(),_b.v());
        }
      };
    }
  }
}
