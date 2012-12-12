#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace object 
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
          typedef Segment::scalar_type scalar_type;
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
      }
    }
  }
}
