#pragma once
#include "tomo/geometry/base/Point.hpp"

namespace tomo
{
	namespace slicing
  {
    struct RepRapState : geometry::Model3f
    {
      TOMO_MODEL_TYPES(geometry::Model3f)
      typedef geometry::base::Point<model_type> point_type;

      TBD_PROPERTY_REF(point_type,pos)
      TBD_PROPERTY(scalar_type,temp)
      TBD_PROPERTY(scalar_type,speed)
      TBD_PROPERTY(scalar_type,extruderLength)
    };
  }
}
