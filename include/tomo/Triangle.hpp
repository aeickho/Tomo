#pragma once

#include "tomo/Vertex.hpp"
#include "tomo/Plane.hpp"

#include <GL/glu.h>

namespace tomo
{
  template<class POINT>
  struct TriangleConcept : public Primitive 
  {    
    virtual Point3f& v0() const = 0;
    virtual Point3f& v1() const = 0;
    virtual Point3f& v2() const = 0;

    virtual bool intersect(Ray& _ray, Vec3f* _normal = NULL, TexCoords* _texCoords = NULL) const 
    {
      Vec3f A = v1() - v0(), 
            B = v2() - v0();
      Vec3f p = _ray.dir_.cross(B);

      // d = Determinant
      float d = A * p;
      if (d > -EPSILON && d < EPSILON) return false;

      float inv_d = 1.0f / d;

      Vec3f tV = _ray.org_ - v0();

      float u = tV * p * inv_d;
      if (u < 0.0 || u > 1.0) return false;

      Vec3f q = tV.cross(A);
      float v = _ray.dir_ * q * inv_d;
      if (v < 0.0 || u + v > 1.0) return false;

      float t = B * q *  inv_d;
      return ray.intersection((Primitive*)this,t,u,v); 
    }  

    virtual bool intersect(Slice& _slice) const
    {

    }

    int intersect(float splitPos, int axis)
    {
      float minPos = min(v0()[axis],min(v1()[axis],v2()[axis]));
      float maxPos = max(v0()[axis],max(v1()[axis],v2()[axis]));

      if (maxPos < splitPos) return 1;
      if (minPos > splitPos) return 2; 
      return 3;
    }

    POINT v[3];
  };

  struct Triangle : public TriangleConcept<Point3f>
  {
    Triangle() 
    {
      v[0](Point3f()); 
      v[1](Point3f()); 
      v[2](Point3f()); 
      n(Vec3f());
    }

    virtual Point3f& v0() const { return v[0]; }
    virtual Point3f& v1() const { return v[1]; }
    virtual Point3f& v2() const { return v[2]; }

    Triangle(Point3f _v0, Point3f _v1, Point3f _v2, Vec3f _n = Vec3f())
    {
      v[0] = _v0; v[1] = _v1; v[2] = _v2; n = _n;
      if (n.sqrLength() == 0.0 )
        n = (v[2] - v[0]).cross(v[1] - v[0]);
    }

    Vec3f n_;

    bool intersect(Ray& _ray, Vec3f* _normal = NULL, TexCoords* _texCoords = NULL) const
    {
      if (intersect(_ray,_normal,_texCoords))
      {
        if (_normal) _normal->set(n);
        if (_texCoords) _texCoords->set(_ray.u_,_ray.v_);
        return true;
      }
      return false;
    }

    bool intersect(Slice& _slice) const;
  };

  struct VertexTriangle : public TriangleConcept<Vertex*>
  {
    VertexTriangle(Vertex* _v0, Vertex* _v1, Vertex* _v2)
    {
      v[0] = _v0; v[1] = _v1; v[2] = _v2; 
    }

    Point3f& v0() const { return &v[0]->v; }
    Point3f& v1() const { return &v[1]->v; }
    Point3f& v2() const { return &v[2]->v; }

    bool intersect(Ray& _ray, Vec3f* _normal = NULL, TexCoords* _texCoords = NULL) const
    {
      if (intersect(_ray,_normal,_texCoords))
      {
        if (_texCoords) _texCoords->set(_ray.u_,_ray.v_);
        if (_normal) _normal->set((1.0 - _ray.u_ - _ray.v_)*v0()->n + _ray.u_*v1()->n + _ray.v_*v2()->n);
        return true;
      }
      return false;
    }
  };

}
