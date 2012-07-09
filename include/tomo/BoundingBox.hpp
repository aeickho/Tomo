#pragma once

#include "Primitive.hpp"

namespace tomo 
{
  /** @brief A bounding is primitive which defines bounds of a compound object
   */
  struct BoundingBox : public Primitive, public Bounds
  {
    BoundingBox() : Bounds() {}
    BoundingBox(Point3f _min, Point3f _max) : Bounds(_min,_max) {}

    bool intersect(Ray& _ray, float& _tNear, float &_tFar, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const;
    
    Bounds bounds() const { return Bounds(min(),max()); }
    
    /** @brief Test if point is in box
     */
    bool pointInBox(Point3f p) const;
  };
}
