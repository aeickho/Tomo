#pragma once
#include "PointNode.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace intersect
    {
      template<class VERTEX>
      struct VertexNode
      {
        typedef VERTEX Vertex;
        typedef typename Vertex::point_type point_type;
        typedef kd::NodeGeometry<typename Vertex::model_type> NodeGeometry;

        kd::NodeIntersectResult operator()(const Vertex& _v,
                                           const NodeGeometry& _n)
        {
          return PointNode<point_type>()(_v.v(),_n);
        }
      }; 
    }
  }
}
