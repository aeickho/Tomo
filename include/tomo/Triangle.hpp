#pragma once

#include "tomo/Vertex.hpp"
#include "tomo/Plane.hpp"

namespace tomo
{
  struct Triangle : public Primitive
  {
    Triangle(Vertex* _v0 = NULL, Vertex* _v1 = NULL, Vertex* _v2 = NULL)
    {
      v[0] = _v0; v[1] = _v1; v[2] = _v2;
    }
  
    Vertex *v[3];

    bool intersect(Ray &ray) const;

    std::vector<Triangle> split(const Plane& plane);

    int splitPlaneIntersect(float splitPos, int axis);

    Vec3f normal(const Ray& ray) const ;
    TexCoords texCoords(const Ray& ray) const { return TexCoords(ray.u,ray.v); }

    void draw(Color color = Color()) const;
    inline void drawStub() const;
  };

}
