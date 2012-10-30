#pragma once

#include "Node.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      template <typename PRIMITIVE>
      struct ContainedNode : Node<PRIMITIVE>
      {
        PRIMITIVE* primitive_;
      };
    }
  }
}
