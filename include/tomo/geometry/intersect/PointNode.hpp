#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace intersect
    {
      template <typename POINT>
      struct PointNode
      {
        typedef POINT Point;
        typedef kd::NodeGeometry<typename Point::model_type> NodeGeometry;

        kd::NodeIntersectResult operator()(const Point& _p,
                                       const NodeGeometry& _n)
        {
          return kd::NodeIntersectResult(_p[_n.axis()] <= _n.splitPos(),
                                         _p[_n.axis()] >= _n.splitPos());
        }
      };
    }
  }
}
