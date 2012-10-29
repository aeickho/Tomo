#pragma once

#include "tomo/geometry/prim/Vertex.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace visitor
      {
        template<class VERTEX>
        struct VertexVertexSqrDistance
        {
          typedef typename VERTEX::scalar_type scalar_type;
          scalar_type operator()(const VERTEX& _a, const VERTEX& _b)
          {
            return (_a.v() - _b.v()).sqrLength();
          } 
        };

        template<class VERTEX>
        struct VertexBoundsSqrDistance
        {
          typedef typename VERTEX::scalar_type scalar_type;
          typedef typename VERTEX::bounds_type bounds_type;

          scalar_type operator()(const VERTEX& _v, const bounds_type& _b)
          {
            if (_b.inside(_v.v())) return 0;

            scalar_type _minDist = INF;
            TOMO_FOREACH_DIM_(_v.v(),i)
            {
              scalar_type _d = (_v.v()[i] < _b.min()[i]) ? _b.min()[i] : _b.max()[i];
              _d -= _v.v()[i];
              _d *= _d;
              _minDist = std::min(_d,_minDist);
            }
            return _minDist*_minDist;
          }
        };

        template<class VERTEX>
        struct VertexNodeIntersector
        {
          typedef typename kd::NodeGeometry<typename VERTEX::model_type> Node;

          NodeIntersectResult operator()(const VERTEX& _v, const Node& _n)
          {
            return NodeIntersectResult(_v.v()[_n.axis()] <= _n.splitPos(),
                                       _v.v()[_n.axis()] >= _n.splitPos());
          }
        };
      }
    }
  }
}
