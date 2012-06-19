#pragma once

#include "tomo/Vertex.hpp"
#include "tomo/Plane.hpp"

#include <GL/glu.h>

namespace tomo
{
  struct Triangle : public Primitive
  {
    Triangle() 
    {
      v[0](Point3f()); 
      v[1](Point3f()); 
      v[2](Point3f()); 
      n(Vec3f());
    }

    Triangle(Point3f _v0, Point3f _v1, Point3f _v2, Vec3f _n = Vec3f())
    {
      v[0] = _v0; v[1] = _v1; v[2] = _v2; n = _n;
      if (n.sqrLength() == 0.0 )
        n = (v[2] - v[0]).cross(v[1] - v[0]);
    }
  
    Vec3f n;
    Point3f v[3];

    bool intersect(Ray &ray) const;

    int splitPlaneIntersect(float splitPos, int axis);

    Vec3f normal(const Ray& ray) const ;
    TexCoords texCoords(const Ray& ray) const { return TexCoords(ray.u_,ray.v_); }
  };
}
