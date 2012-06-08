#include "tomo/Mesh.hpp"

#include <boost/foreach.hpp>
#include "tomo/OFFReader.hpp"

#include <tbd/log.h>

#include <GL/glu.h>

using namespace std;

namespace tomo
{
  void Mesh::calcBoundingBox()
  {
    boundingBox.min.set(INF,INF,INF);
    boundingBox.max.set(-INF,-INF,-INF);

    BOOST_FOREACH( Triangle& tri, triangles )
      for (int i = 0; i < 3; i++)
      {
        Point3f v = tri.v[i];
        FOREACH_AXIS
        {
          boundingBox.min[axis] = std::min(v[axis],boundingBox.min[axis]);
          boundingBox.max[axis] = std::max(v[axis],boundingBox.max[axis]);
        }
      }
  }

  void Mesh::displayNormals()
  {
    BOOST_FOREACH( Triangle& tri, triangles )
    {
      Vec3f n = boundingBox.size().length()*0.02f*tri.n; 
      Vec3f mid = (1.0f / 3.0f) * ( tri.v[0].vec3f() + tri.v[1].vec3f() + tri.v[2].vec3f());

      glPushMatrix();
      glTranslatef(COORDS(mid));
      glBegin(GL_LINES);
      glColor3f( 0.0, 1.0, 0.0);
      glVertex3f( 0.0 , 0.0 , 0.0 );
      glVertex3fv(n.p());
      glColor3f( 0.0, 0.0, 0.0);
      glEnd();
      glPopMatrix();
    }
  }

  TriangleKDTree::TriangleKDTree()
  {
  }

  void Mesh::draw(Color color) const
  {
    glColor3f(color.x,color.y,color.z);
    
    glBegin(GL_TRIANGLES);
    BOOST_FOREACH( const Triangle& tri, triangles ) 
      tri.drawStub();

    glEnd();
 }

  void TriangleKDTree::divideNode(Node* node, const BoundingBox& box, int depth)
  {
    if (depth > 15 || node->objs.size() < 10)
    {   // We have a leaf node!
      node->left = NULL; node->right = NULL;
      return;
    }
    node->left = new Node;
    node->right = new Node;
    node->axis = box.dominantAxis();

    // TODO: Surface Area Heuristic here!
    node->splitPos = 0.5*(box.min.cell[node->axis] + box.max.cell[node->axis]);
    BoundingBox boxLeft, boxRight;
    box.split(node->splitPos,node->axis,boxLeft,boxRight);
    
    BOOST_FOREACH( Triangle* tri, node->objs )
    {
      int result = tri->splitPlaneIntersect(node->splitPos,node->axis);
      if (result & 1) node->left->objs.push_back(tri);
      if (result & 2) node->right->objs.push_back(tri);
    }
    node->objs.clear();
    divideNode(node->left,boxLeft,depth+1);
    divideNode(node->right,boxRight,depth+1);
  }

  void Mesh::read(string filename)
  {
    OFFReader off;
    Vertices vertices;
    off.read(filename,&vertices,&triangles);
    calcBoundingBox();
    calcNormals();
    kdTree.build(triangles,boundingBox);
  }


  float TriangleKDTree::recKDTreeTraverse(Ray& ray, Node* node, float tnear, float tfar, bool& found) const
  {
    if (node->isLeaf())
    {
      BOOST_FOREACH( Triangle* tri, node->objs )
        if (tri != ray.obj) found |= (tri->intersect(ray));
      return ray.tmax;
    }

    int k = node->axis;
    float d = (node->splitPos - ray.org[k]) / ray.dir[k];

    KDNode<Triangle>* front = node->left;
    KDNode<Triangle>* back  = node->right;
    if (ray.dir[k] < 0) swap(front,back); 

    if (d <= tnear)
    {
      recKDTreeTraverse(ray,back,tnear,tfar,found);
    } else
      if (d >= tfar)
      {
        recKDTreeTraverse(ray,front,tnear,tfar,found);
      } else
      {
        float t_hit = recKDTreeTraverse(ray,front,tnear,d,found);
        if (t_hit <= d) return t_hit;
        return recKDTreeTraverse(ray,back,d,tfar,found);
      }
    return INF;
  }

  bool Mesh::intersect(Ray& ray) const
  {
    bool found = false;
    kdTree.recKDTreeTraverse(ray,kdTree.root,ray.tmin,ray.tmax,found);
    return found;
  }

  std::pair<Mesh,Mesh> Mesh::split(const Plane& splitPlane)
  {
    std::pair<Mesh,Mesh> halves;
    
    BOOST_FOREACH ( Triangle& tri, triangles )
      splitTriangle(tri,splitPlane,halves.first,halves.second);

    halves.first.calcBoundingBox();
    halves.first.kdTree.build(halves.first.triangles,halves.first.boundingBox);    
    halves.second.calcBoundingBox();
    halves.second.kdTree.build(halves.second.triangles,halves.first.boundingBox);

    return halves;
  }

  void Mesh::splitTriangle(const Triangle& tri, const Plane& plane, Mesh& behind, Mesh& front)
  {
    vector<Triangle> triangles;
    Point3f V[3] = tri.v; 

    int signCount = 0;
    bool signs[3];

    for (int i = 0; i < 3; i++)
    {
      signs[i] = (V[i] - plane._c) * plane._n;
      signCount += int(signs[i] < 0);
    }

    vector<Triangle> *q = &behind.triangles, *r = &front.triangles;
    if (signCount >= 2) swap(q,r);

    if (signCount == 0 || signCount == 3) 
    {
      r->push_back(tri);
      return;
    }

    int k = 0;
    for (int i = 1; i < 3; i++)
      if ((signCount == 1) ? signs[i] : !signs[i]) k = i;
    int u = (k+1) % 3, v = (k+2) % 3;

    Vec3f A = V[u] - V[k], B = V[v] - V[k]; 
    Point3f iPoint[2];
    iPoint[0] = V[k] + A*(plane._n * (plane._c - V[k]) / (A * plane._n));
    iPoint[1] = V[k] + B*(plane._n * (plane._c - V[k]) / (B * plane._n));

    q->push_back(Triangle(V[k],iPoint[0],iPoint[1],tri.n));
    r->push_back(Triangle(V[u],V[v],iPoint[0],tri.n));
    r->push_back(Triangle(V[u],iPoint[0],iPoint[1],tri.n));
  }

}
