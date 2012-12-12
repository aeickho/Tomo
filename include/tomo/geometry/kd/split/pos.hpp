#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace split
      {
        template<typename PRIMITIVE>
        typename PRIMITIVE::scalar_type pos(const PRIMITIVE& _primitive, base::Axis _axis)
        {
          return 0.0;
        }

        using prim::Segment;
        template<>
        Segment::scalar_type pos(const Segment& _segment, base::Axis _axis)
        {
          return _segment.p0()[_axis];
        }

        using prim::Vertex2f;
        template<>
        Vertex2f::scalar_type pos(const Vertex2f& _vertex, base::Axis _axis)
        {
          return _vertex.v()[_axis];
        }
      }
    }
  }
}
