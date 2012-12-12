#pragma once

#include "tomo/geometry/kd/Tree.hpp"

#include "BuildPolicy.hpp"
#include "StaticContainer.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      namespace nearest
      {
        template<typename PRIMITIVE>
        struct Tree : 
          kd::Tree<StaticContainer<PRIMITIVE>,BuildPolicy<PRIMITIVE>>
        {
          typedef PRIMITIVE primitive_type;
          typedef BuildPolicy<PRIMITIVE> build_policy_type;
          typedef StaticContainer<PRIMITIVE> container_type;
          typedef kd::Tree<container_type,build_policy_type> base_type;
          typedef typename build_policy_type::state_type state_type;
          TOMO_INHERIT_STATE_TYPES(state_type)

          Tree(const typename state_type::input_type& _input) : base_type(_input) {}
        };
      }
    }
  }
}
