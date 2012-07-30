#pragma once

#include "Primitive.hpp"

namespace tomo 
{
  /** @brief A bounding is primitive which defines bounds of a compound object
   */
  struct BoundingBox : public Primitive3f, public Bounds3f
  {
    BoundingBox() : Bounds3f() {}
    BoundingBox(Point3f _min, Point3f _max) : Bounds3f(_min,_max) {}

    bool intersect(Ray3f& _ray, float& _tNear, float &_tFar, Vec3f* _normal = NULL) const;
    
    Bounds3f bounds() const { return Bounds3f(min(),max()); }
  };
}
