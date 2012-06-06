#pragma once

#include "tomo/Mesh.hpp"
#include "tomo/KDTree.hpp"

#include <set>

namespace tomo
{
  struct SelectedPoint
  {
    SelectedPoint(float _dist, Vertex* _v) : dist(_dist), v(_v) {}
    float dist;
    Vertex* v;
  };

  struct PointCompare
  {
    bool operator() (const SelectedPoint& a, const SelectedPoint& b) 
    {
      return a.dist < b.dist;
    }
  };

  class PointSet : public std::set<SelectedPoint,PointCompare>
  {
    public:
    PointSet(Point3f _center, float _radius = 0.0, int _k = 0);

    bool insert(Vertex* v);
    set<const Vertex*> vertexSet();
    float maxDist();

    TBD_PROPERTY(int,k);
    TBD_PROPERTY(float,radius);
    TBD_PROPERTY_REF(Point3f,center); 
  };

  class PointKDTree : public KDTree<Vertex>
  {
  public:
    PointKDTree() : drawDepth_(10) {}
    void collect(KDNode<Vertex>* node, const BoundingBox& box, PointSet& pointSet);

    TBD_PROPERTY(unsigned,drawDepth);
  private:
    void divideNode(KDNode<Vertex>* node, const BoundingBox& box, int depth);

    float nodeDistance(const Point3f& p, const BoundingBox& box) const;
  };


  class PointCloud : public Mesh
  {
    public:
      PointCloud();

      void read(string filename);
      void write(string filename);

      void draw(Color color = Color()) const;

      Vec3f normal(const Ray& ray) { return Vec3f(); }
      TexCoords texCoords(const Ray& ray) { return TexCoords(); }

      bool intersect(Ray& ray) { return false; }


      void update();
      void collectKNearest(Point3f& p, int k);
      void collectInRadius(Point3f& p, float radius);

      set<const Vertex*> selection;

      TBD_PROPERTY(bool,drawKDTree);
      TBD_PROPERTY_REF(Color,kdTreeColor);
      TBD_PROPERTY(bool,drawBoundingBox);
      TBD_PROPERTY_REF(Color,boundingBoxColor);

      TBD_PROPERTY(bool,highlightSelection);
      TBD_PROPERTY(Color,selectionColor);
    private:
      PointKDTree kdTree;
  };
}
