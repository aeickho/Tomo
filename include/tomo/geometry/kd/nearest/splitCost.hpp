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
        float splitCost(const PRIMITIVE* _p)
        {
          return 1.0;
        }

        using prim::Segment;
        template<>
        float splitCost(const Segment* _s)
        {
          return 1.0;
        }

        using prim::Vertex2f;
        template<>
        float splitCost(const Vertex2f* _v)
        {
          return 0.9;
        }
      }
    }
  }
}
