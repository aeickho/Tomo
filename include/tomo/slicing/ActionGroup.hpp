#pragma once

#include "Action.hpp"

namespace tomo
{
  namespace slicing
  {
    template<class ACTION> class ActionGroup 
    {
      typedef typename ACTION::State State;
      typedef typename ACTION::Point3f Point3f;

      Point3f begin() const
      {
        return actions_.front().begin();
      }
      Point3f end() const
      {
        return actions_.back().end();
      }

      TBD_PROPERTY_REF(std::vector<ACTION>,actions);
    };
  }
}
