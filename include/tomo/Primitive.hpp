#pragma once

#include "tomo/Object.hpp"
#include "tomo/Ray.hpp"

namespace tomo 
{
  /* A primitive is an object which is shadable and for which an intersection point can be found 
   */
  struct Primitive
  {
    virtual bool intersect(Ray& _ray, Vec3f* _normal = NULL, TexCoords* _texCoords = NULL) const = 0;
    virtual bool intersect(Slice& _slice) const = 0;

    Primitive* pointer() const { return const_cast<Primitive*>(this); }

// TBD_PROPERTY(Shader*,shader);
  };
}
