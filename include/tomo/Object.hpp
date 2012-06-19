#pragma once

#include "tomo/misc.hpp"
#include "tomo/Vector.hpp"
#include <vector>

namespace tomo 
{
  struct Object
  {
  };

  typedef std::vector<Object*> ObjectList;
  typedef std::vector<Object> Objects;
}
