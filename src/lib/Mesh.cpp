#include "tomo/Mesh.hpp"

#include <boost/foreach.hpp>
#include "tomo/OFFReader.hpp"

#include <tbd/log.h>

#include <GL/glu.h>

namespace tomo
{
  void Mesh::calcBoundingBox()
  {
    boundingBox.min.set(INF,INF,INF);
    boundingBox.max.set(-INF,-INF,-INF);

    BOOST_FOREACH( Vertex& vertex, vertices )
    {
      Point3f v = vertex.v;
      FOREACH_AXIS
      {
        if (v.cell[axis] > boundingBox.max.cell[axis]) boundingBox.max.cell[axis] = v.cell[axis];
        if (v.cell[axis] < boundingBox.min.cell[axis]) boundingBox.min.cell[axis] = v.cell[axis];
      }
    }
  }

  void Mesh::displayNormals()
  {
    BOOST_FOREACH( Vertex& vertex, vertices )
    {
      Vec3f n = boundingBox.size().length()*0.02f*vertex.n; 

      glPushMatrix();
      glTranslatef(COORDS(vertex.v));
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

  void TriangleMesh::draw(Color color)
  {
    glColor3f(color.x,color.y,color.z);
    
    glBegin(GL_TRIANGLES);
    BOOST_FOREACH( Triangle& tri, triangles ) 
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

  void TriangleMesh::read(string filename)
  {
    OFFReader off;
    off.read(filename,&vertices,&triangles);
    calcBoundingBox();
    calcNormals();
    kdTree.build(triangles,boundingBox);
  }


  float TriangleKDTree::recKDTreeTraverse(Ray& ray, Node* node, float tnear, float tfar, bool& found)
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

  bool TriangleMesh::intersect(Ray& ray)
  {
    bool found = false;
    kdTree.recKDTreeTraverse(ray,kdTree.root,ray.tmin,ray.tmax,found);
    return found;
  }

  std::pair<TriangleMesh,TriangleMesh> TriangleMesh::split(const Plane& splitPlane)
  {
    std::pair<TriangleMesh,TriangleMesh> halves;
    
    BOOST_FOREACH ( Triangle& tri, triangles )
      splitTriangle(tri,splitPlane,halves.first,halves.second);

    halves.first.calcBoundingBox();
    halves.first.kdTree.build(halves.first.triangles,halves.boundingBox);    
    halves.second.calcBoundingBox();
    halves.second.kdTree.build(halves.second.triangles,halves.boundingBox);

    return halves;
  }

  void TriangleMesh::splitTriangle(const Triangle& tri, const Plane& plane, TriangleMesh& behind, TriangleMesh& front)
  {
    vector<Triangle> triangles;

    Vertex* v[3] = tri.v;
    int signCount = 0;
    bool signs[3];

    for (int i = 0; i < 3; i++)
    {
      signs[i] = (v[i]->v - plane._c) * plane._n;
      signCount += int(signs[i] < 0);
    }
    
    if (signCount == 3)
    {
      behind.
    }


    behind.


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
