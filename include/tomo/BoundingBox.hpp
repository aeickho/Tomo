#pragma once

#include "Primitive.hpp"

namespace tomo 
{
  struct BoundingBox : public Primitive
  {
    bool intersect(Ray& ray) const;
    void set(const Point3f& _min, const Point3f& _max) { min = _min; max = _max; }
    void draw(Color color = Color());

    void split(float splitPos, Axis axis, BoundingBox& boxLeft, BoundingBox& boxRight);

    Vec3f normal(const Ray& ray) const { return Vec3f(); }
    TexCoords texCoords(const Ray& ray) const { return TexCoords(); }

    Axis dominantAxis();

    bool pointInBox(Point3f p);

    Vec3f size() { return max-min; }

    // Bounding box points
    Point3f min, max;
  };
}
