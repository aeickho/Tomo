#pragma once

#include "tomo/geometry/kd/split/CostFunction.hpp"
#include "tomo/geometry/kd/BuildPolicy.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace object
      {
        template
        <
          typename STATE,
          typename SPLITPOS,
          typename SPLITCOST
        >
        struct BuildPolicy : 
          kd::BuildPolicy<
            STATE,
            split::DominantAxis<STATE>,
            split::CostFunction<STATE,SPLITPOS,SPLITCOST>>
        {
        };
      }
    }
  }
}
