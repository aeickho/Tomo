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
        typedef POINT point_type;
        typedef kd::NodeGeometry<typename point_type::model_type> node_geometry_type;

        kd::NodeIntersectResult operator()(const point_type& _p,
                                       const node_geometry_type& _n)
        {
          return kd::NodeIntersectResult(_p[_n.axis()] <= _n.splitPos(),
                                         _p[_n.axis()] >= _n.splitPos());
        }
      };
    }
  }
}
