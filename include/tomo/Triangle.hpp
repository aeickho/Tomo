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

    virtual bool intersect(ray_type& _ray, float& _tNear, float &_tFar, Vec3f* _normal = NULL) const 
    {
      Vec3f c = v0() - v1(), 
            b = v2() - v1(),
            N = cross(b,c);
      /*
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
      */

      // distance test
      float t_plane = - dot((_ray.org()-v1()),N) / dot(_ray.dir(),N);
      if (t_plane < _tNear || t_plane > _tFar) return false;

      // determine projection dimensiondimensions
      Axis k = N.dominantAxis();
      int u = (k+1) % 3, v = (k+2) % 3;

      // calc hitpoint
      float Hu = _ray.org()[u] + t_plane * _ray.dir()[u] - v1()[u];
      float Hv = _ray.org()[v] + t_plane * _ray.dir()[v] - v1()[v];

      float beta = (b[u] * Hv - b[v] * Hu) / (b[u] * c[v] - b[v] * c[u]);

      if (beta < 0) return false;

      float gamma = (c[v] * Hu - c[u] * Hv) / (b[u] * c[v] - b[v] * c[u]);

      if (gamma < 0) return false;
      if (beta+gamma > 1.0) return false;

      if (_ray.intersection(this->pointer(),t_plane,_tNear,_tFar))
      {
        if (_normal) (*_normal)(normal(NULL));
        return true;
      }
      return false;
    }

    SplitPlaneIntersect intersect(Axis _axis, float _splitPos, const Bounds3f& _boundsLeft, const Bounds3f& _boundsRight) const
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

    void slice(Slices& _slices) const 
    {
      Point3f A = v0(), B = v1(), C = v2();
      typename Slices::const_iterator _Ait = _slices.get(A.z()), 
               _Bit = _slices.get(B.z()), 
               _Cit = _slices.get(C.z()),
               it;

      Slice* _sliceA = const_cast<Slice*>(&(*_Ait));
      Slice* _sliceB = const_cast<Slice*>(&(*_Bit));
      Slice* _sliceC = const_cast<Slice*>(&(*_Cit));

      // If all vertices lay in the same slice, add a triangle
      if ((_Ait == _Bit) && (_Bit == _Cit)) 
      {  
        _sliceA->addSegment(A,B,Vec3f());
        _sliceA->addSegment(A,C,Vec3f());
        _sliceA->addSegment(B,C,Vec3f());
      }

      /// Sort vertices and corresponding slice iterators,
      /// So that A is the lower, B in the middle and C the upper vertex
      if (_sliceA->posZ_ > _sliceB->posZ_) { std::swap(_sliceA,_sliceB); std::swap(_Ait,_Bit); std::swap(A,B); }
      if (_sliceB->posZ_ > _sliceC->posZ_) { std::swap(_sliceB,_sliceC); std::swap(_Bit,_Cit); std::swap(B,C); }
      if (_sliceA->posZ_ > _sliceB->posZ_) { std::swap(_sliceA,_sliceB); std::swap(_Ait,_Bit); std::swap(A,B); }

      Vec3f b = B - A;
      Vec3f c = C - A;
      Vec3f d = C - B;
      Vec3f N = normal();

      for (it = _Ait ; it != _Cit && it != _slices.end() ; ++it)
      {
        Slice* _slice = const_cast<Slice*>(&(*it));

        float _ratioR = (_slice->posZ_ - A.z()) / c.z();
        Vec3f r(c.x()*_ratioR,c.y()*_ratioR,_slice->posZ_);
        Vec3f s;

        if (_slice->posZ_ - A.z() < b.z())
        {
          float _ratioS = (_slice->posZ_ - A.z()) / b.z();
          s(b.x()*_ratioS,b.y()*_ratioS,_slice->posZ_);
        } else
        {
          float _ratioS = (_slice->posZ_ - B.z()) / d.z();
          s = b + Vec3f(d.x()*_ratioS,d.y()*_ratioS,_slice->posZ_);
        }
        _slice->addSegment(A+r,A+s,N);
      }
    }

    Bounds3f bounds() const
    {
      return Bounds3f(Point3f(std::min(v0().x(),std::min(v1().x(),v2().x())),
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

  struct VertexTriangle : public TriangleConcept<Vertex3f*>
  {
    VertexTriangle(Vertex3f* _v0, Vertex3f* _v1, Vertex3f* _v2)
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
    Vertex3f* v_[3];
  };

}
