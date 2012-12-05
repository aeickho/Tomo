#pragma once

#include "BuildState.hpp"
#include "tomo/geometry/kd/BuildPolicy.hpp"
#include "tomo/geometry/kd/split/CostFunction.hpp"
#include "tomo/geometry/kd/split/DominantAxis.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace nearest
      {
        template
        <
          typename BUILD_STATE,
          typename SPLITPOS,
          typename SPLITCOST
        >
        struct BuildPolicy
        {
          typedef BUILD_STATE state_type;
          TOMO_INHERIT_STATE_TYPES(state_type)

          typedef split::DominantAxis<BUILD_STATE> splitaxis_type;
          typedef split::CostFunction<BUILD_STATE,SPLITPOS,SPLITCOST> splitpos_type;

          bool split(state_type& _state)
          {
            splitaxis_type()(_state);
            splitpos_type _splitPos;

            if (_splitPos(_state))
            {
              _state.primitive(_splitPos.primitive());
              return true;
            }
            return false;
          }
        };
      }
    }
  }
}
