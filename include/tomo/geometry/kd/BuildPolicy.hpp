#pragma once

namespace tomo
{
  namespace geometry
  {
    namespace kd
    {
      template
      <
        typename STATE,
        typename SPLITAXIS,
        typename SPLITPOS
      >
      struct BuildPolicy
      {
        typedef STATE state_type;
        TOMO_INHERIT_STATE_TYPES(state_type)

        /// Split function
        bool split(state_type& _state)
        {
          SPLITAXIS()(_state);
          return SPLITPOS()(_state);
        }

        attr_type nodeAttributes(state_type& _state)
        {
        }

        inner_attr_type innerNodeAttributes(state_type& _state)
        {
        }

        leaf_attr_type leafNodeAttributes(state_type& _state)
        {
        }

      };
    }
  }
}

