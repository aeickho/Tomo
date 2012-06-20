#pragma once

#include "Primitive.hpp"

namespace tomo 
{
  struct BoundingBox : public Primitive, public Bounds
  {
    BoundingBox() : Bounds(Point3f(INF,INF,INF),Point3f(-INF,-INF,-INF)) {}
    BoundingBox(Point3f _min, Point3f _max) : Bounds(_min,_max) {}

    bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const;
    
    void split(float splitPos, Axis axis, BoundingBox& boxLeft, BoundingBox& boxRight) const;

    Bounds bounds() const { return Bounds(min(),max()); }

    Axis dominantAxis() const;
    bool pointInBox(Point3f p) const;
  };
}
