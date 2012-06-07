#include "tomo/Triangle.hpp"
#include "tomo/Ray.hpp"

#include "tbd/log.h"

#include <GL/glu.h>

namespace tomo 
{
  using std::min;
  using std::max;

  bool Triangle::intersect(Ray &ray) const
  {
    Vec3f A = v[1] - v[0], 
          B = v[2] - v[0];
    Vec3f p = ray.dir.cross(B);

    // d = Determinant
    float d = A * p;
    if (d > -EPSILON && d < EPSILON) return 0;

    float inv_d = 1.0f / d;

    Vec3f tV = ray.org - v[0]->v;

    float u = tV * p * inv_d;
    if (u < 0.0 || u > 1.0) return false;

    Vec3f q = tV.cross(A);
    float v = ray.dir * q * inv_d;
    if (v < 0.0 || u + v > 1.0) return false;

    float t = B * q *  inv_d;

    ray.t(t);
    ray.texCoord(u,v);
    ray.normal = normal();
    ray.obj = const_cast<Triangle*>(this);
    return true;
  }

  int Triangle::splitPlaneIntersect(float splitPos, int axis)
  {
    float minPos = min(v[0]->v[axis],min(v[1]->v[axis],v[2]->v[axis]));
    float maxPos = max(v[0]->v[axis],max(v[1]->v[axis],v[2]->v[axis]));

    if (maxPos < splitPos) return 1;
    if (minPos > splitPos) return 2; 
    return 3;
  }

  inline void Triangle::drawStub() const
  {
    glNormal3fv(normal);	 
    for (int i  = 0; i < 3; i++)
      glVertex3fv(v[i].p());
  }

  void Triangle::draw(Color color) const
  {
    glColor3fv(color.p());
    glBegin(GL_TRIANGLES);
    drawStub();
    glEnd();
  }

  Vec3f Triangle::normal(const Ray& ray) const
  {
    return n.normalized();
  }

}
