#pragma once

#include "tomo/geometry/kd/split/CostFunction.hpp"
#include "tomo/geometry/kd/BuildPolicy.hpp"
#include "splitCost.hpp"
#include "BuildState.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace object
      {
        namespace
        {
          /// Functor for determining the cost of each primitive when making a split
          template<typename PRIMITIVE>
          struct SplitCost
          {
            typedef PRIMITIVE primitive_type;
            float operator()(const primitive_type* _p)
            {
              return splitCost(_p);
            }
          };
        }

        template<typename PRIMITIVE>
        using BuildPolicy = 
          kd::BuildPolicy
          <
            BuildState<PRIMITIVE>,
            split::DominantAxis<BuildState<PRIMITIVE>>,
            split::CostFunction< BuildState<PRIMITIVE>,SplitCost<PRIMITIVE> >
          >;
      }
    }
  }
}
