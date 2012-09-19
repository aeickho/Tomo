#include "tomo/geometry/prim/Sphere.hpp"

#include <cmath>

using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {

      /*	void Sphere::draw(Color color) const
      	{
      		glPushMatrix();
      		glTranslatef(center_[0], center_[1], center_[2]);
      		glutSolidSphere(radius_, 32, 32);
      		glPopMatrix();
      	}
      */
      bool Sphere::intersect(ray_type& _ray, scalar_type& _tNear, scalar_type&_tFar, vector_type* _normal) const
      {
        vector_type o = _ray.org() - center_;
        scalar_type a = dot(_ray.dir(),_ray.dir());
        scalar_type b = 2.0f * dot(_ray.dir(),o);
        scalar_type c = dot(o,o) - radius()*radius();
        scalar_type disc = b*b - 4*a*c;

        if (disc < 0) return false;

        scalar_type discSqrt = sqrtf(disc);
        scalar_type q = (b < 0) ? (-b - discSqrt) : (-b + discSqrt);
        q *= 0.5f;

        scalar_type t0 = q/a, t1 = c/q;
        if (t0 > t1) std::swap(t0,t1);
        if (t1 < 0) return false;

        if (_normal)
        {
          (*_normal)((_ray.intersectionPoint() - center_).normalized());
          //   if (_texCoords)
          //   (*_texCoords)(atan2(_normal->x(),_normal->z()) / (2.0*M_PI) + 0.5,acosf(_normal->y())/M_PI);
        }
        return _ray.intersection(Primitive3f::pointer(),(t0 < 0) ? t1 : t0,_tNear,_tFar);
      }

      Sphere::bounds_type Sphere::bounds() const
      {
        point_type _rP(radius_,radius_,radius_),
                   _rM(-radius_,-radius_,-radius_);
        return bounds_type(center_ + _rM, center_ + _rP);
      }

    }
  }
}
