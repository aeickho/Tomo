#pragma once

#include "Action.hpp"

namespace tomo
{
  namespace slicing
  {
    template<class ACTION> class ActionGroup 
    {
      TBD_PROPERTY_REF(std::vector<ACTION>,actions);
    };
  }
}
