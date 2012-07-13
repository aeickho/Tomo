#include "tomo/Mesh.hpp"

#include <boost/foreach.hpp>

#include <tbd/log.h>

#include <GL/glu.h>

using namespace std;

namespace tomo
{
  void Mesh::read(const string& _filename)
  {
    if (!OpenMesh::IO::read_mesh(*this, _filename))
    {
      LOG_ERR << "Read error.";
      return;
    }

//    LOG_MSG << fmt("Read '%' which has % vertices.") % _filename % n_vertices();
    update_face_normals();
    update_vertex_normals();

    Mesh::ConstFaceIter    fIt(faces_begin()),fEnd(faces_end());
    Mesh::ConstFaceVertexIter fvIt;

    for (; fIt!=fEnd; ++fIt)
    {
      fvIt = cfv_iter(fIt.handle());
      Triangle _triangle(point(fvIt),point(++fvIt),point(++fvIt),normal(fIt));
      objs_.push_back(_triangle);
    }
    calcBoundingBox();

    build(objs_,boundingBox_);
  }

  bool Mesh::intersect(Ray3f& _ray, float& _tNear, float &_tFar, Vec3f* _normal) const
  {
    return traversal(_ray,boundingBox_,_normal);
  /* 
   bool _found = false;
   BOOST_FOREACH ( const Triangle& _triangle, objs_ )
      _found |= _triangle.intersect(_ray,_ray.tNear(),_ray.tFar(),_normal,_texCoords);
   return _found;
*/
  }

  void Mesh::slice(Slices& _slices) const
  {
    BOOST_FOREACH ( const Triangle& _triangle, objs_ )
      _triangle.slice(_slices);
  }

  std::pair<Mesh,Mesh> Mesh::split(const Plane& splitPlane)
  {
    /*
       std::pair<Mesh,Mesh> halves;

       BOOST_FOREACH ( Triangle& tri, objs_ )
       splitTriangle(tri,splitPlane,halves);

       halves.first.calcBoundingBox();
       halves.first.build(halves.first.objs_,halves.first.boundingBox_);    
       halves.second.calcBoundingBox();
       halves.second.build(halves.second.objs_,halves.first.boundingBox_);
       */
    return std::pair<Mesh,Mesh>();
  }

  void Mesh::splitTriangle(const Triangle& tri, const Plane& plane, 
      std::pair<Mesh,Mesh>& _halves)
  {
    // TODO To be adapted for OpenMesh
    /* 
       vector<Triangle> triangles;
       Point3f V[3]; 
       for (int i = 0; i < 3; i++)
       V[i] = tri.v[i];

       int signCount = 0;
       bool signs[3];

       for (int i = 0; i < 3; i++)
       {
       signs[i] = dot(V[i] - plane.center_,plane.normal_);
       signCount += int(signs[i] < 0);
       }

       vector<Triangle> *q = &_halves.first.objs_, *r = &_halves.second.objs_;
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
       iPoint[0] = V[k] + A*(dot(plane.normal_,plane.center_ - V[k]) / dot(A,plane.normal_));
       iPoint[1] = V[k] + B*(dot(plane.normal_,plane.center_ - V[k]) / dot(A,plane.normal_));

       q->push_back(Triangle(V[k],iPoint[0],iPoint[1],tri.normal()));
       r->push_back(Triangle(V[u],V[v],iPoint[0],tri.normal()));
       r->push_back(Triangle(V[u],iPoint[0],iPoint[1],tri.normal()));*/
  }

}
