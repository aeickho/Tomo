#pragma once

#include "tomo/Object.hpp"
#include "tomo/Shader.hpp"
#include "tomo/Ray.hpp"

namespace tomo 
{
  /* A primitive is an object which is shadable and for which an intersection point can be found 
   */
  struct Primitive: public Object
  {
    virtual Vec3f normal(const Ray& ray) const = 0;
    virtual TexCoords texCoords(const Ray& ray) const = 0;

    virtual bool intersect(Ray& ray) const = 0;
   
    Primitive* pointer() const { return const_cast<Primitive*>(this); }

    TBD_PROPERTY(Shader*,shader);
  };
}
