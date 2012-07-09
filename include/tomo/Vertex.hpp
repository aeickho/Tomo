#pragma once

#include "tomo/Primitive.hpp"

#include <vector>
#include <set>

namespace tomo 
{
  using std::vector;
  using std::set;

  struct Vertex : public Primitive
  {
    Vertex(Point3f _v = Point3f(), Vec3f _n = Vec3f()) { v(_v); n(_n); }
    void set(Point3f _v, Vec3f _n = Vec3f()) { v = _v; n = _n; }

    bool intersect(Ray& _ray, float& _tNear, float &_tFar, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const 
    {
      return false;
    }

    SplitPlaneIntersect intersect(Axis _axis, float _splitPos, const Bounds& _boundsLeft, const Bounds& _boundsRight) const
    {
      return Primitive::intersect(_axis,_splitPos,_boundsLeft,_boundsRight);
    }

    Bounds bounds() const { return Bounds(v,v); }

    Point3f v;
    Vec3f n;
    bool hasNormal() { return (n.length() != 0.0f); }

  };

  typedef vector<Vertex> Vertices;
  typedef vector<Vertex*> VertexList;
  typedef std::set<Vertex*> VertexSet;

  struct Polygon : public VertexList
  {
    Polygon(int n = 3) { if (n < 3) n = 3; resize(n); } 
	  Vec3f normal();
  };

  typedef vector<Polygon*> PolygonList;
  typedef vector<Polygon> Polygons;
}
