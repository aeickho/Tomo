#pragma once

#include "Action.hpp"

namespace tomo
{
  namespace slicing
  {
    template<class ACTION, class CONT=std::vector<ACTION> > class ActionGroup 
    {
      typedef typename ACTION::State State;
      typedef typename ACTION::Point3f Point3f;
      typedef CONT Cont;

      Point3f begin() const
      {
        return actions_.front().begin();
      }
      Point3f end() const
      {
        return actions_.back().end();
      }

      TBD_PROPERTY_REF(Cont,actions);
    };
  }
}
