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
          typedef VERTEX Vertex;
          typedef typename kd::NodeGeometry<typename Vertex::model_type> NodeGeometry;
          typedef typename Vertex::scalar_type scalar_type;
          typedef typename Vertex::bounds_type bounds_type;

          NodeIntersectResult operator()(const Vertex& _v, const NodeGeometry& _n)
          {
            return NodeIntersectResult(_v.v()[_n.axis()] <= _n.splitPos(),
                                       _v.v()[_n.axis()] >= _n.splitPos());
          }

          template <typename NODE>
          void nodeSetup(NodeGeometry& _nodeGeometry,
                         NODE* _node, 
                         Vertex*& _primitive)
          {
            base::Axis _axis = _nodeGeometry.bounds().dominantAxis();
            scalar_type _splitPos = 0.5*(_nodeGeometry.bounds().min()[_axis] + _nodeGeometry.bounds().max()[_axis]);

            _nodeGeometry.splitPos(_splitPos);
            _nodeGeometry.axis(_axis);
          }
        };
      }
    }
  }
}
