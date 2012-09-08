#pragma once

#include "tomo/Primitive.hpp"

namespace tomo 
{
  struct Plane : Primitive3f
  {
    Plane(const Point3f& _center, const Vec3f& _normal)
      : center_(_center), normal_(_normal)
    {}

    bool intersect(Ray3f& _ray, float& _tNear, float &_tFar, Vec3f* _normal = NULL) const;

    Bounds3f bounds() const;

    Point3f center_;
    Vec3f normal_;
  };
}
