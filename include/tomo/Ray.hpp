#pragma once 
#include "tomo/Vector.hpp"

#include <vector>

namespace tomo
{
  struct Primitive;

  struct Ray
  {
    Ray(const Point3f _org = Point3f(), const Vec3f _dir = Vec3f(), float _tNear = 0.002, float _tFar = INF): 
      org_(_org), dir_(_dir), tNear_(_tNear), tFar_(_tFar), primitive_(NULL)  {  }

    bool intersection(Primitive* _primitive, float _t, float _tNear, float _tFar) 
    {
      if (_t >= _tNear && _t < _tFar)
      {
      primitive_ = _primitive;
      tFar_ = _t;
      return true;
      }
      return false;
    }

    inline bool intersection(Primitive* _primitive, float _t)
    {
      return intersection(_primitive,_t,tNear_,tFar_);
    }

    Point3f intersectionPoint() const
    {
      return org_ + dir_ * tNear_;
    }

    void params(Point3f _org, Vec3f _dir) 
    {
      org_=_org; dir_=_dir;
    }

    TBD_PROPERTY_REF(Point3f,org);
    TBD_PROPERTY_REF(Vec3f,dir);
    TBD_PROPERTY_REF(float,tNear);
    TBD_PROPERTY_REF(float,tFar);

  private:
    Primitive* primitive_;
  };
}

