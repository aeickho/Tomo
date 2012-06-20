#pragma once

#include "tomo/Ray.hpp"
#include "tomo/Bounds.hpp"

namespace tomo 
{
  /* A primitive is an object which is shadable and for which an intersection point can be found 
  */
  struct Primitive
  {
    virtual bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const = 0;

    Primitive* pointer() const { return const_cast<Primitive*>(this); }

    virtual Bounds bounds() const = 0;

    // TBD_PROPERTY(Shader*,shader);
  };
}
