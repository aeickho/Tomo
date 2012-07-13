#pragma once

#include "tomo/Vertex.hpp"
#include "tomo/Compound.hpp"

#include <map>

namespace tomo
{
  class PointSet
  {
  public:
    std::multimap<double, Vertex3f *> points;
    PointSet(Point3f _center, float _radius = std::numeric_limits<float>::max(), int _k = std::numeric_limits<int>::max());

    virtual bool insert(Vertex3f* v);
    std::set<const Vertex3f*> vertexSet();
    float maxDist();

    TBD_PROPERTY(int,k)
    TBD_PROPERTY(float,radius)
    TBD_PROPERTY_REF(Point3f,center)
  };

  class PointCloud : public Compound<Vertex3f>
  {
    public:
      typedef KDNode<Vertex3f> Node; 
      typedef std::vector<Vertex3f*> PrimCont;
      typedef std::vector<Node> NodeCont;

      PointCloud();

      void read(const string& filename);

      bool intersect(Ray3f& _ray, float& _tNear, float &_tFar, Vec3f* _normal = NULL) const { return false; }

      void update();
      void collectKNearest(Point3f& p, int k);
      void collectInRadius(Point3f& p, float radius);

      virtual std::set<const Vertex3f*> collectKNearest(Point3f const & p, int k) const;
      virtual std::set<const Vertex3f*> collectInRadius(Point3f const & p, float radius) const;
      bool isNearest(const Vertex3f& _v, const Point3f& _p);

      std::set<const Vertex3f*> selection;

      std::vector<Vertex3f> vertices_;

      void collect(Node* node, BoundingBox& box, PointSet& pointSet);

      TBD_PROPERTY(bool,drawKDTree);
      TBD_PROPERTY(bool,drawBoundingBox);
    
    private:
      float splitPos(const PrimCont& _primList, NodeInner* _inner, const Bounds3f& _bounds) const;
      float nodeDistance(const Point3f& p, const BoundingBox& box) const;
  };
}
