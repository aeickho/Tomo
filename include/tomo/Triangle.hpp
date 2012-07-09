#pragma once

#include "tomo/Vertex.hpp"
#include "tomo/SlicableObject.hpp"

namespace tomo
{
  template<class POINT>
  struct TriangleConcept : public SlicableObject 
  {    
    virtual const Point3f& v0() const = 0;
    virtual const Point3f& v1() const = 0;
    virtual const Point3f& v2() const = 0;
    virtual const Vec3f normal(Point2f* _texCoords = NULL) const = 0;

    virtual bool intersect(Ray& _ray, float& _tNear, float &_tFar, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const 
    {
      Vec3f A = v1() - v0(), 
            B = v2() - v0();
      Vec3f p = cross(_ray.dir(),B);

      // d = Determinant
      float d = dot(A,p);
      if (d > -EPSILON && d < EPSILON) return false;

      float inv_d = 1.0f / d;

      Vec3f tV = _ray.org() - v0();

      float u = dot(tV,p) * inv_d;
      if (u < 0.0 || u > 1.0) return false;

      Vec3f q = cross(tV,A);
      float v =  dot(_ray.dir(),q) * inv_d;
      if (v < 0.0 || u + v > 1.0) return false;

      float t = dot(B,q) *  inv_d;

      if (_ray.intersection(this->pointer(),t,_tNear,_tFar))
      {
        if (_texCoords) (*_texCoords)(u,v);
        if (_normal) (*_normal)(normal(_texCoords));
        return true;
      }
      return false;
    }

    SplitPlaneIntersect intersect(Axis _axis, float _splitPos, const Bounds& _boundsLeft, const Bounds& _boundsRight) const
    {
      // TODO implement better split plane intersection function
      SplitPlaneIntersect _result = Primitive::intersect(_axis,_splitPos,_boundsLeft,_boundsRight);

/*      if (_result.both())
      {
        Ray rays[4];
        int k = _axis, u = (k+1) % 3, v = (k+2) % 3;

  


        _boundsRight.min()[k] = 
        _boundsRight.min()[k] = 
        _boundsRight.min()[k] = 
        _boundsRight.min()[k] = 


      }*/

      return _result;
    }

    bool slice(Slice& _slice) const
    {
      return false;
    }

    Bounds bounds() const
    {
      return Bounds(Point3f(std::min(v0().x(),std::min(v1().x(),v2().x())),
                            std::min(v0().y(),std::min(v1().y(),v2().y())),
                            std::min(v0().z(),std::min(v1().z(),v2().z()))),
                    Point3f(std::max(v0().x(),std::max(v1().x(),v2().x())),
                            std::max(v0().y(),std::max(v1().y(),v2().y())),
                            std::max(v0().z(),std::max(v1().z(),v2().z()))));
    }
  };

  struct Triangle : public TriangleConcept<Point3f>
  {
    Triangle() 
    {
      v_[0](Point3f()); 
      v_[1](Point3f()); 
      v_[2](Point3f()); 
      n_(Vec3f());
    }

    Triangle(Point3f _v0, Point3f _v1, Point3f _v2, Vec3f _n = Vec3f())
    {
      v_[0] = _v0; v_[1] = _v1; v_[2] = _v2; n_ = _n;
      if (_n.sqrLength() == 0.0 )
        n_ = cross(v_[2] - v_[0],v_[1] - v_[0]);
    }

    const Point3f& v0() const { return v_[0]; }
    const Point3f& v1() const { return v_[1]; }
    const Point3f& v2() const { return v_[2]; }
    const Vec3f normal(Point2f* _texCoords = NULL) const { return n_; }

    private:
      Point3f v_[3];
      Vec3f n_;
  };

  struct VertexTriangle : public TriangleConcept<Vertex*>
  {
    VertexTriangle(Vertex* _v0, Vertex* _v1, Vertex* _v2)
    {
      v_[0] = _v0; v_[1] = _v1; v_[2] = _v2; 
    }

    const Point3f& v0() const { return v_[0]->v; }
    const Point3f& v1() const { return v_[1]->v; }
    const Point3f& v2() const { return v_[2]->v; }
    const Vec3f normal(Point2f* _texCoords = NULL) const 
    { 
      if (_texCoords)
          return (1.0 - _texCoords->x() - _texCoords->y())*v_[0]->n + 
                  _texCoords->x()*v_[1]->n + _texCoords->y()*v_[2]->n;
      return v_[0]->n; 
    }

    private:
      Vertex* v_[3];
  };

}
