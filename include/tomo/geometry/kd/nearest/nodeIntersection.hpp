#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace nearest
      {
        template<typename PRIMITIVE>
        kd::NodeIntersectResult nodeIntersection(
            const PRIMITIVE& _primitive,
            const kd::NodeGeometry<typename PRIMITIVE::model_type> _node)
        {
          return kd::NodeIntersectResult();
        }

        using base::Point2f;

        template<>
        kd::NodeIntersectResult nodeIntersection(
            const base::Point2f& _p,
            const kd::NodeGeometry<Point2f::model_type> _n)
        {
          return kd::NodeIntersectResult(_p[_n.axis()] <= _n.splitPos(),
                                         _p[_n.axis()] >= _n.splitPos());
        }

        using prim::Segment;

        template<>
        kd::NodeIntersectResult nodeIntersection(
            const Segment& _segment,
            const kd::NodeGeometry<Segment::model_type> _node)
        {
          return nodeIntersection(_segment.p0(),_node);
        }
      }
    }
  }
}
