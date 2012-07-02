#include "tomo/PointCloud.hpp"
#include "tomo/Median.hpp"

#include <boost/foreach.hpp>

#include <cassert>

#include "tomo/OFFReader.hpp"
#include "tomo/OFFWriter.hpp"

#include "tbd/log.h"

using namespace std;

namespace tomo
{
  typedef pair<double,Vertex *> entry;

  PointSet::PointSet(Point3f _center, float _radius, int _k) : k_(_k), radius_(_radius), center_(_center)
  {

  }

  bool PointSet::insert(Vertex * v)
  {
    Vec3f distVec = v->v-center();
    float dist = distVec.length();
    if (dist > radius()) return false;

    if (int(points.size()) >= k())
      if (!points.empty() && (--points.end())->first < dist) return false;

    points.insert(make_pair(dist, v));

    if (points.empty())
      return true;

    multimap<double, Vertex *>::iterator it = points.end();
    it--;
    if (int(points.size()) > k())
    {
      points.erase(it);
    }
    return true;
  }


  set<const Vertex*> PointSet::vertexSet()
  {
    set<const Vertex*> result;
    BOOST_FOREACH( const entry& p, points )
      result.insert(p.second);
    return result;
  }

  float PointSet::maxDist()
  {
    if (!k_ && radius_ > 0.0f)
    {
      return radius_;
    }

    if (k_)
    {
      if (k_ < int(points.size()) && radius_ >  0.0f)
      {
        return radius_;
      }
      if (k_ >= int(points.size()) && !points.empty())
      {
        multimap<double,Vertex *>::iterator it = points.end();
        it--;
        return it->first;
      }
    }
    return INF;
  }
 
  static bool compareX(const Vertex* a, const Vertex* b) { return a->v.x() < b->v.x(); }
  static bool compareY(const Vertex* a, const Vertex* b) { return a->v.y() < b->v.y(); }
  static bool compareZ(const Vertex* a, const Vertex* b) { return a->v.z() < b->v.z(); }
  
  float PointCloud::splitPos(const PrimCont& _primList, const NodeInner& _inner, const Bounds& _bounds) const
  {
    switch (_inner.axis())
    {
      case X: return median(_primList,compareX)->v.x(); 
      case Y: return median(_primList,compareY)->v.y(); 
      case Z: return median(_primList,compareZ)->v.z(); 
      default: return 0.0;
    }
  }


  void PointCloud::collect(Node* node, const BoundingBox& box, PointSet& pointSet) 
  {
    if (!node) return;

    if (node->isLeaf())
    {
      BOOST_FOREACH( Vertex* vertex, node->leaf_.primitives(primLists_) )
        pointSet.insert(vertex);
      return;
    }
    
    BoundingBox boxLeft, boxRight;
    box.split(node->inner_.splitPos(),node->inner_.axis(),boxLeft,boxRight);
    
    if (nodeDistance(pointSet.center(),boxLeft) < pointSet.maxDist()) 
      collect(&nodes_[node->inner_.left()],boxLeft,pointSet);
    if (nodeDistance(pointSet.center(),boxRight) < pointSet.maxDist())
      collect(&nodes_[node->inner_.right()],boxRight,pointSet);
  }

  float PointCloud::nodeDistance(const Point3f& p, const BoundingBox& box) const
  {
    if (box.pointInBox(p)) return 0.0;

    float minDist = INF;
    FOREACH_AXIS 
    {
      minDist = std::min(std::abs(p[axis] - box.min()[axis]),std::abs(box.max()[axis] - p[axis]));
    }

    return minDist;
  }


  PointCloud::PointCloud() : drawKDTree_(false), 
                             //kdTreeColor_(Color(0.0,0.2,0.4)), 
                             drawBoundingBox_(false)
                             //boundingBoxColor_(Color(0.0,1.0,0.0)),
                             //selectionColor_(Color(1.0,1.0,1.0))
  {

  }

  void PointCloud::read(const string& filename)
  {
    OFFReader off;
    off.read(filename,&vertices_,NULL);
    update();
  }

  void PointCloud::write(const string& filename) const
  {
    OFFWriter off;
    off.write(filename,&vertices_,NULL);
  }

  void PointCloud::update()
  {
    calcBoundingBox();
    build(vertices_,boundingBox_);
  }

  void PointCloud::collectKNearest(Point3f& p, int k)
  {
    PointSet pointSet(p,0.0,k);
    collect(root(),boundingBox_,pointSet);
    selection = pointSet.vertexSet();
  }

  void PointCloud::collectInRadius(Point3f& p, float radius)
  {
    PointSet pointSet(p,radius); 
    collect(root(),boundingBox_,pointSet);
    selection = pointSet.vertexSet();
  }

  bool PointCloud::isNearest(const Vertex& _v, const Point3f& _p)
  {
    float radius = (_v.v - _p).length();
    return collectInRadius(_p,radius).size() <= 1;
  }

}
