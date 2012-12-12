#pragma once

#include "tomo/geometry/prim/Segment.hpp"
#include "tomo/geometry/prim/Vertex.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace object 
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
          return 0.8;
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
