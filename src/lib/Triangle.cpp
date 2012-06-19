#include "tomo/Triangle.hpp"
#include "tomo/Ray.hpp"

#include "tbd/log.h"


namespace tomo 
{
  using std::min;
  using std::max;

  bool Triangle::intersect(Ray &ray) const
  {
    Vec3f A = v[1] - v[0], 
          B = v[2] - v[0];
    Vec3f p = ray.dir_.cross(B);

    // d = Determinant
    float d = A * p;
    if (d > -EPSILON && d < EPSILON) return 0;

    float inv_d = 1.0f / d;

    Vec3f tV = ray.org_ - v[0];

    float u = tV * p * inv_d;
    if (u < 0.0 || u > 1.0) return false;

    Vec3f q = tV.cross(A);
    float v = ray.dir_ * q * inv_d;
    if (v < 0.0 || u + v > 1.0) return false;

    float t = B * q *  inv_d;

    return ray.intersection((Primitive*)this,t,u,v);
  }

  int Triangle::splitPlaneIntersect(float splitPos, int axis)
  {
    float minPos = min(v[0][axis],min(v[1][axis],v[2][axis]));
    float maxPos = max(v[0][axis],max(v[1][axis],v[2][axis]));

    if (maxPos < splitPos) return 1;
    if (minPos > splitPos) return 2; 
    return 3;
  }

  Vec3f Triangle::normal(const Ray& ray) const
  {
    return n.normalized();
  }

}
