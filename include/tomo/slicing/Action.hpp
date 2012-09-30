#pragma once

#include "tomo/geometry/base/Point.hpp"

namespace tomo
{
  namespace slicing
  {
    template<class State> class Action
    {
    public:
      //    typedef State::Costs Costs;
        /// Generate GCode
      //    virtual GCode generate( State& _state ) = 0;
        virtual Point3f begin() const = 0;
        virtual Point3f end() const = 0;
      //    virtual Costs costs() const = 0;
    };
  }

}
