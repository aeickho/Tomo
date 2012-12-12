#pragma once

#include "BuildState.hpp"
#include "splitCost.hpp"
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
        template<typename PRIMITIVE>
        struct BuildPolicy
        {
          typedef BuildState<PRIMITIVE> state_type;
          TOMO_INHERIT_STATE_TYPES(state_type)

          typedef PRIMITIVE primitive_type;

          /// Functor for determining the cost of each primitive when making a split
          struct SplitCost
          {
            float operator()(const primitive_type* _p)
            {
              return splitCost(_p);
            }
          };

          typedef split::DominantAxis<state_type> splitaxis_type;
          typedef split::CostFunction<state_type,SplitCost> splitpos_type;

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
