#pragma once

#include "tomo/geometry/base/Point.hpp"

namespace tomo
{
  namespace slicing
  {
    /** @brief concept of an action
     *  @tparam STATE slicing session state type
     */
    template<class STATE> struct Action
    {
      typedef geometry::base::Point3f Point3f;
      typedef STATE State;
      Point3f begin() const;
      Point3f end() const;
    };
  }

}
