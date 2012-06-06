#pragma once

#include "Primitive.hpp"

namespace tomo 
{
  struct BoundingBox : public Primitive
  {
    bool intersect(Ray& ray) const;
    void set(const Point3f& _min, const Point3f& _max) 
    { 
      FOREACH_AXIS 
        if (min.cell[axis] > max.cell[axis]) 
          std::swap(min.cell[axis],max.cell[axis]);
      min = _min; 
      max = _max; 
    
    }
    void draw(Color color = Color()) const;

    void split(float splitPos, Axis axis, BoundingBox& boxLeft, BoundingBox& boxRight) const;

    Vec3f normal(const Ray& ray) const { return Vec3f(); }
    TexCoords texCoords(const Ray& ray) const { return TexCoords(); }

    Axis dominantAxis() const;

    bool pointInBox(Point3f p) const;

    Vec3f size() { return max-min; }

    // Bounding box points
    Point3f min, max;
  };
}
