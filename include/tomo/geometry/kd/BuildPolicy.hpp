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

        /**@brief Split function, return true if node can be split, 
         * @detail Returns false if node cannot be split and node 
         *         will be leaf node consequently)
         */
        bool split(state_type& _state)
        {
          SPLITAXIS()(_state);
          return SPLITPOS()(_state);
        }
      };
    }
  }
}

