#pragma once

#include "Action.hpp"

namespace tomo
{
  namespace slicing
  {
    template<class ACTION, class STATE> class ActionGroup : public ACTION<STATE>
    {
      typedef ACTION<STATE> Action;
      TBD_PROPERTY_REF(std::vector<Action>,actions);
    };
  }
}
