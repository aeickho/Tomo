#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace intersect
    {
      struct SegmentNode
      {
        typedef prim::Segment Segment;
        typedef Segment::point_type point_type;
        typedef Segment::vec_type vec_type;
        typedef Segment::scalar_type scalar_type;
        typedef kd::NodeGeometry<Segment::model_type> node_geometry_type;

        kd::NodeIntersectResult operator()(const Segment& _segment,
                                           const node_geometry_type& _node)
        {
         // TOMO_NOT_TESTED();

          const base::Axis& _axis = _node.axis();
          const scalar_type& _splitPos = _node.splitPos();

          if (_segment[0][_axis] < _splitPos &&
              _segment[1][_axis] < _splitPos)
          {
            return kd::NodeIntersectResult(true,false);
          } else
          if (_segment[0][_axis] > _splitPos &&
              _segment[1][_axis] > _splitPos)
          {
            return kd::NodeIntersectResult(false,true);
          } else
          {
/*            kd::NodeIntersectResult _result;
            base::Axis _orthAxis = base::Axis(1 - int(_axis));

            /// Split plane points
            point_type _sP0,_sP1;
            _sP0[_axis] = _sP1[_axis] = _splitPos;
            _sP0[_orthAxis] = _node.bounds().min()[_orthAxis];
            _sP1[_orthAxis] = _node.bounds().max()[_orthAxis];

            _sP0 -= _segment[0];
            _sP1 -= _segment[0];

            vec_type _n = _segment.normal();
            scalar_type _dot0 = dot(_n,_sP0.vec());
            scalar_type _dot1 = dot(_n,_sP1.vec());

            if ((_dot0 <  0 && _dot1 >= 0) ||
                (_dot0 >= 0 && _dot1 <  0)) return kd::NodeIntersectResult(true,true);
*/
            return kd::NodeIntersectResult(true,true);
          }
        }
      };
    }
  }
}
