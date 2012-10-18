#pragma once
#include <tomo/misc.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      namespace intersect
      {
        template <class MODEL_A, class MODEL_B>
        struct Intersector
        {
          typedef MODEL_A::scalar_type scalar_type;
          typedef MODEL_A::vec_type vec_type;
          typedef MODEL_A::point_type point_type;
        };
      }
    }
  }
}
