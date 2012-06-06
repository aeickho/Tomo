#pragma once

#include "tomo/Object.hpp"
#include "tomo/Shader.hpp"
#include "tomo/Ray.hpp"

namespace tomo 
{
  struct Primitive: public Object
  {
    virtual Vec3f normal(const Ray& ray) const = 0;
    virtual TexCoords texCoords(const Ray& ray) const = 0;

    virtual bool intersect(Ray& ray) const = 0;
    
    TBD_PROPERTY(Shader*,shader);
  };

  DEFINE_CONTAINERS(Primitive)
}
