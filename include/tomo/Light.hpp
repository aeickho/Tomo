#pragma once 

#include "tomo/Vector.hpp"

#include <vector>

namespace tomo
{
  struct Light
  {
    TBD_DECLARE_PROPERTY_REF(Point3f,pos);
    TBD_DECLARE_PROPERTY_REF(Color,ambient);
    TBD_DECLARE_PROPERTY_REF(Color,diffuse);
    TBD_DECLARE_PROPERTY_REF(Color,specular);
    TBD_DECLARE_PROPERTY(float,intensity);
    TBD_DECLARE_PROPERTY(float,shadows);
    TBD_DECLARE_PROPERTY(float,radius);
  };

  typedef std::vector<Light*> LightList;
  typedef std::vector<Light> Lights;
}

