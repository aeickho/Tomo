#include "tomo/Plane.hpp"

namespace tomo 
{
  bool Plane::intersect(Ray3f& _ray, float& _tNear, float &_tFar, Vec3f* _normal) const
  {
    float dn = dot(_ray.dir(),normal_);
    if (dn == 0.0f) return false;

    Vec3f org = _ray.org() - center_;
    float d = -dot(normal_,org) / dn;
    if (d < 0) return false;

    Vec3f iPoint = _ray.org() +_ray.dir() *d - center_;

    if (_normal) (*_normal)(normal_);

    return _ray.intersection(this->pointer(),d,_tNear,_tFar);
  }

  Bounds3f Plane::bounds() const
  {
    return Bounds3f(Point3f(-INF,-INF,-INF),Point3f(INF,INF,INF));
  }
}
