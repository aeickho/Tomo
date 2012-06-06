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
    Vec3f A = v[1]->v - v[0]->v, 
          B = v[2]->v - v[0]->v;
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
    ray.normal = normal(ray);
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
    for (int i  = 0; i < 3; i++)
    {
      glNormal3fv(v[i]->n.p());	 
      glVertex3fv(v[i]->v.p());
    }
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
    Vec3f n = (1.0f - ray.u - ray.v)*v[0]->n + ray.u*v[1]->n + ray.v*v[2]->n;
    return n.normalized();
  }

  vector<Triangle> Triangle::split(const Plane& plane)
  {
    vector<Triangle> triangles;

    Ray rayAB(v[0]->v,v[1]->v - v[0]->v), 
        rayAC(v[0]->v,v[2]->v - v[0]->v), 
        rayBC(v[0]->v,v[2]->v - v[1]->v);
    rayAB.tmin = 0.0f; rayAB.tmax = 1.0f;
    rayAC.tmin = 0.0f; rayAC.tmax = 1.0f;
    rayBC.tmin = 0.0f; rayBC.tmax = 1.0f;

    typedef enum { AB, AC, BC, NONE } Edge;

    unsigned idx = 0; 
    Point3f iPoint[2]; Edge edge[2];
    edge[0] = NONE; edge[1] = NONE;

    if (plane.intersect(rayAB))
    {
      iPoint[idx] = rayAB.getIntersectionPoint(); 
      edge[idx] = AB;
      idx++;
    }

    if (plane.intersect(rayAC))
    {
      iPoint[idx] = rayAC.getIntersectionPoint(); 
      edge[idx] = AC;
      idx++;
    }

    if (idx < 2)
      if (plane.intersect(rayBC))
      {
        iPoint[idx] = rayBC.getIntersectionPoint(); 
        edge[idx] = BC;
        idx++;
      }

    switch (idx)
    {
      // No intersection point found, return input triangle
      case 0: 
        triangles.push_back(Triangle(v[0],v[1],v[2])); break;
      case 1:
        {
          Vertex* vertex = new Vertex(iPoint[0]);
          switch (edge[0])
          {
            case AB: 
              triangles.push_back(Triangle(v[0],vertex,v[2]));
              triangles.push_back(Triangle(vertex,v[1],v[2]));
              vertex->n = rayAB.tmax*v[0]->n + (1.0f-rayAB.tmax)*v[1]->n; 
              break;
            case AC: 
              triangles.push_back(Triangle(v[0],v[1],vertex));
              triangles.push_back(Triangle(vertex,v[1],v[2]));
              vertex->n = rayAC.tmax*v[0]->n + (1.0f-rayAC.tmax)*v[2]->n; 
              break;
            case BC: 
              triangles.push_back(Triangle(v[0],vertex,v[2]));
              triangles.push_back(Triangle(v[0],v[1],vertex));
              vertex->n = rayBC.tmax*v[1]->n + (1.0f-rayBC.tmax)*v[2]->n; 
              break;
            default: break; 
          }
        }
      case 2: 
        {
          Vertex* v0 = new Vertex(iPoint[0]);
          Vertex* v1 = new Vertex(iPoint[1]);
          switch (edge[0])
          {
            case AB: v0->n = rayAB.tmax*v[0]->n + (1-rayAB.tmax)*v[1]->n; break;
            case AC: v0->n = rayAC.tmax*v[0]->n + (1-rayAC.tmax)*v[2]->n; break;
            case BC: v0->n = rayBC.tmax*v[1]->n + (1-rayBC.tmax)*v[2]->n; break;
            default: break;
          }
          switch (edge[1])
          {
            case AB: v1->n = rayAB.tmax*v[0]->n + (1-rayAB.tmax)*v[1]->n; break;
            case AC: v1->n = rayAC.tmax*v[0]->n + (1-rayAC.tmax)*v[2]->n; break;
            case BC: v1->n = rayBC.tmax*v[1]->n + (1-rayBC.tmax)*v[2]->n; break;
            default: break;
          }

          if (edge[0] == AB && edge[1] == AC)
          {
            triangles.push_back(Triangle(v[0],v0,v1));
            triangles.push_back(Triangle(v0,v1,v[2]));
            triangles.push_back(Triangle(v0,v[1],v[2]));
            return triangles;
          }
          if (edge[0] == AB && edge[1] == BC)
          {
            triangles.push_back(Triangle(v0,v[1],v1));
            triangles.push_back(Triangle(v[0],v0, v[2]));
            triangles.push_back(Triangle(v0,v1,v[2]));
            return triangles;
          }
          if (edge[0] == AC && edge[1] == BC)
          {
            triangles.push_back(Triangle(v0,v1,v[2]));
            triangles.push_back(Triangle(v[0],v[1],v0));
            triangles.push_back(Triangle(v[0],v0,v1));
            return triangles;
          }
        }
        return triangles;
    }


  }
}
