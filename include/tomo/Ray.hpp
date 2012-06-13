#pragma once 
#include "tomo/Vector.hpp"

#include <vector>

namespace tomo
{
  class Primitive;

  struct Ray
  {
    Ray(const Point3f _org = Point3f(), const Vec3f _dir = Vec3f(), float _tMin = 0.002, float _tMax = 1000000.0): 
      org_(_org), dir_(_dir), tMin_(_tMin), tMax_(_tMax),  u_(0.0), v_(0.0), primitive_(NULL) {  }

    bool intersection(Primitive* _primitive, float _t, float _u, float _v) 
    {
      if (_t >= tMin_ && _t < tMax_)
      {
      primitive_ = _primitive;
      tMin_ = _t; u_ = _u; v_ = _v;
      return true;
      }
      return false;
    }

    Point3f intersectionPoint() const
    {
      return org_ + dir_ * tMin_;
    }

    void params(Point3f _org, Vec3f _dir) 
    {
      org_=_org; dir_=_dir;
    }
    Ray reflect();
    Ray refract(float index);

    Point3f org_;
    Vec3f dir_;
   
    Primitive* primitive_;
    float tMin_, tMax_;
    float u_,v_;

  };

  typedef std::vector<Ray*> RayList;
  typedef std::vector<Ray> Rays;

}

