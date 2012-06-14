#include "tomo/PointCloud.hpp"
#include "tomo/Median.hpp"

#include <boost/foreach.hpp>

#include <cassert>
#include <GL/glut.h>

#include "tomo/OFFReader.hpp"
#include "tomo/OFFWriter.hpp"

#include "tbd/log.h"

using namespace std;

namespace tomo
{
  PointSet::PointSet(Point3f _center, float _radius, int _k) : k_(_k), radius_(_radius), center_(_center)
  {

  }

  bool PointSet::insert(Vertex* v)
  {
    float dist = (v->v-center()).length();
    if (radius_ > 0.0f && dist > radius_) return false;

    if (k_ && int(size()) >= k_)
      if ((--end())->dist < dist) return false;

    SelectedPoint p(dist,v);
    std::set<SelectedPoint,PointCompare>::insert(p);

    PointSet::iterator it = end(); it--;
    if (k_ && int(size()) > k_ && !empty())
        erase(it);
    return true;
  }

  set<const Vertex*> PointSet::vertexSet()
  {
    set<const Vertex*> result;
    BOOST_FOREACH( const SelectedPoint& p, *this )
      result.insert(p.v);
    return result;
  }

  float PointSet::maxDist()
  {
    if (!k_ && radius_ > 0.0f) return radius_;

    if (k_)
    {
      if (k_ < int(size()) && radius_ >  0.0f) return radius_;
      if (k_ >= int(size()) && !empty()) 
      {
        PointSet::iterator it = end(); it--;
        return it->dist;
      }
    }
    return INF;
  }
 
  static bool compareX(const Vertex* a, const Vertex* b) { return a->v.x() < b->v.x(); }
  static bool compareY(const Vertex* a, const Vertex* b) { return a->v.y() < b->v.y(); }
  static bool compareZ(const Vertex* a, const Vertex* b) { return a->v.z() < b->v.z(); }

  void PointKDTree::collect(KDNode<Vertex>* node, const BoundingBox& box, PointSet& pointSet)
  {
    if (!node) return;

    if (node->isLeaf())
    {
      BOOST_FOREACH( Vertex* vertex, node->objs )
        pointSet.insert(vertex);
      return;
    }
    
    BoundingBox boxLeft, boxRight;
    box.split(node->splitPos,node->axis,boxLeft,boxRight);
    
    if (nodeDistance(pointSet.center(),boxLeft) < pointSet.maxDist()) 
      collect(node->left,boxLeft,pointSet);
    if (nodeDistance(pointSet.center(),boxRight) < pointSet.maxDist())
      collect(node->right,boxRight,pointSet);
  }
  void PointKDTree::divideNode(KDNode<Vertex>* node, const BoundingBox& box, int depth)
  {
    assert(node);
    LOG_MSG_(2) << fmt("Depth: %, objs: %") % depth % node->objs.size();

    if (depth > 20 || node->objs.size() < 4)
    {   // We have a leaf node!
      node->left = NULL; node->right = NULL;
      return;
    }
    node->left = new KDNode<Vertex>();
    node->right = new KDNode<Vertex>();
    node->axis = box.dominantAxis();

    switch (node->axis)
    {
      case X: node->splitPos = median(node->objs,compareX)->v.x(); break;
      case Y: node->splitPos = median(node->objs,compareY)->v.y(); break;
      case Z: node->splitPos = median(node->objs,compareZ)->v.z(); break;
      default: return;
    }
    BoundingBox boxLeft, boxRight;
    box.split(node->splitPos,node->axis,boxLeft,boxRight);
    
    BOOST_FOREACH( Vertex* vertex, node->objs )
    {
      KDNode<Vertex>* subNode = (vertex->v[node->axis] < node->splitPos) ? node->left : node->right;
      subNode->objs.push_back(vertex);
    }
    node->objs.clear();
    divideNode(node->left,boxLeft,depth+1);
    divideNode(node->right,boxRight,depth+1);
  }

  float PointKDTree::nodeDistance(const Point3f& p, const BoundingBox& box) const
  {
    if (box.pointInBox(p)) return 0.0;

    float minDist = INF;
    FOREACH_AXIS 
    {
      minDist = std::min(std::abs(p[axis] - box.min[axis]),std::abs(box.max[axis] - p[axis]));
    }

  //  LOG_MSG << minDist;
    return minDist;
  }


  PointCloud::PointCloud() : drawKDTree_(false), 
                             kdTreeColor_(Color(0.0,0.2,0.4)), 
                             drawBoundingBox_(false),
                             boundingBoxColor_(Color(0.0,1.0,0.0)),
                             selectionColor_(Color(1.0,1.0,1.0))
  {

  }

  void PointCloud::read(string filename)
  {
    OFFReader off;
    off.read(filename,&vertices,NULL);
    update();
  }

  void PointCloud::write(string filename)
  {
    OFFWriter off;
    off.write(filename,&vertices,NULL);
  }

  void PointCloud::update()
  {
    calcBoundingBox();
    kdTree.build(vertices,boundingBox_);
  }

  void PointCloud::draw(Color color) const
  {
    if (drawBoundingBox_ && !drawKDTree_) boundingBox_.draw(boundingBoxColor());
    if (drawKDTree_) kdTree.draw(kdTreeColor(),boundingBox_);

    glBegin(GL_POINTS);
    BOOST_FOREACH( const Vertex& vertex, vertices )
    {
      if (selection.count(&vertex))
        glColor3f(COORDS(selectionColor()));
      else
        glColor3f(COORDS(color));
      glVertex3f(COORDS(vertex.v));
    }
    glEnd(); 
  }


  void PointCloud::collectKNearest(Point3f& p, int k)
  {
    PointSet pointSet(p,0.0,k);
    kdTree.collect(kdTree.root,boundingBox_,pointSet);
    selection = pointSet.vertexSet();
  }

  void PointCloud::collectInRadius(Point3f& p, float radius)
  {
    PointSet pointSet(p,radius); 
    kdTree.collect(kdTree.root,boundingBox_,pointSet);
    selection = pointSet.vertexSet();
  }

}
