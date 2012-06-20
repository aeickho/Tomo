#pragma once

#include "tomo/Vertex.hpp"
#include "tomo/Compound.hpp"

#include <map>

namespace tomo
{
  class PointSet
  {
  public:
    std::multimap<double, Vertex *> points;
    PointSet(Point3f _center, float _radius = std::numeric_limits<float>::max(), int _k = std::numeric_limits<int>::max());

    virtual bool insert(Vertex * v);
    set<Vertex const *> vertexSet();
    float maxDist();

    TBD_PROPERTY(int,k)
    TBD_PROPERTY(float,radius)
    TBD_PROPERTY_REF(Point3f,center)
  };

  class PointCloud : public Compound<Vertex>
  {
    public:
      PointCloud();

      void read(const string& filename);
      void write(const string& filename) const;

      bool intersect(Ray& _ray, Vec3f* _normal = NULL, Point2f* _texCoords = NULL) const { return false; }

      void update();
      void collectKNearest(Point3f& p, int k);
      void collectInRadius(Point3f& p, float radius);

      virtual set<const Vertex*> collectKNearest(Point3f const & p, int k) const;
      virtual set<const Vertex *> collectInRadius(Point3f const & p, float radius) const;
      bool isNearest(const Vertex& _v, const Point3f& _p);

      set<const Vertex*> selection;

      Vertices vertices_;

      void collect(KDNode<Vertex>* node, const BoundingBox& box, PointSet& pointSet);

      TBD_PROPERTY(bool,drawKDTree);
      TBD_PROPERTY(bool,drawBoundingBox);
    
    private:
      virtual void divideNode(KDNode<Vertex>* node, const BoundingBox& box, int depth);
      float nodeDistance(const Point3f& p, const BoundingBox& box) const;
  };
}
