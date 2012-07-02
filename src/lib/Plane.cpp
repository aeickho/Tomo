#include "tomo/Plane.hpp"

namespace tomo 
{
  bool Plane::intersect(Ray& _ray, Vec3f* _normal, Point2f* _texCoords) const
  {
    float dn = dot(_ray.dir_,normal_);
    if (dn == 0.0f) return false;

    Vec3f org = _ray.org_ - center_;
    float d = -dot(normal_,org) / dn;
    if (d < 0) return false;

    Vec3f iPoint = _ray.org_+_ray.dir_*d - center_;

    if (_normal) (*_normal)(normal_);

    return _ray.intersection(this->pointer(),d);
  }

  Bounds Plane::bounds() const
  {
    return Bounds(Point3f(-INF,-INF,-INF),Point3f(INF,INF,INF));
  }
}
