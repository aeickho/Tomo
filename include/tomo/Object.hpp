#pragma once

#include "tomo/misc.hpp"
#include "tomo/Vector.hpp"
#include <vector>

namespace tomo 
{
  struct Object
  {
    virtual void draw(Color color = Color()) = 0;
  };

  typedef std::vector<Object*> ObjectList;
  typedef std::vector<Object> Objects;
}
