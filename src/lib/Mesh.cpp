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

    calcBounds();
  }

  void Mesh::calcBounds()
  {
    Bounds3f _bounds;
    Mesh::ConstFaceIter    fIt(faces_begin()),fEnd(faces_end());
    Mesh::ConstFaceVertexIter fvIt;
    for (; fIt!=fEnd; ++fIt)
    {
      fvIt = cfv_iter(fIt.handle());
      _bounds.extend(point(fvIt));
      _bounds.extend(point(++fvIt));
      _bounds.extend(point(++fvIt));
    }
    bounds_=_bounds;
  }

  bool Mesh::intersect(Ray3f& _ray, float& _tNear, float &_tFar, Vec3f* _normal) const
  {
    return false; //traversal(_ray,boundingBox_,_normal);
  /* 
   bool _found = false;
   BOOST_FOREACH ( const Triangle& _triangle, objs_ )
      _found |= _triangle.intersect(_ray,_ray.tNear(),_ray.tFar(),_normal,_texCoords);
   return _found;
*/
  }

  void Mesh::slice(Slices& _slices) const
  {
    Mesh::ConstFaceIter    fIt(faces_begin()),fEnd(faces_end());
    for (; fIt!=fEnd; ++fIt)
      sliceTriangle(fIt,_slices);
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

  void Mesh::splitTriangle(ConstFaceIter _faceIter, const Plane& plane, 
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

  void Mesh::sliceTriangle(ConstFaceIter _faceIter, Slices& _slices) const
  {
    Mesh::ConstFaceVertexIter fvIt = cfv_iter(_faceIter.handle());
      Point3f A = point(fvIt), B = point(++fvIt), C = point(++fvIt);
      typename Slices::const_iterator _Ait = _slices.get(A.z()), 
               _Bit = _slices.get(B.z()), 
               _Cit = _slices.get(C.z()),
               it;

      Slice* _sliceA = const_cast<Slice*>(&(*_Ait));
      Slice* _sliceB = const_cast<Slice*>(&(*_Bit));
      Slice* _sliceC = const_cast<Slice*>(&(*_Cit));

      // If all vertices lay in the same slice, add a triangle
      if ((_Ait == _Bit) && (_Bit == _Cit)) 
      {  
        _sliceA->addSegment(A,B,Vec3f());
        _sliceA->addSegment(A,C,Vec3f());
        _sliceA->addSegment(B,C,Vec3f());
      }

      /// Sort vertices and corresponding slice iterators,
      /// So that A is the lower, B in the middle and C the upper vertex
      if (_sliceA->posZ_ > _sliceB->posZ_) { std::swap(_sliceA,_sliceB); std::swap(_Ait,_Bit); std::swap(A,B); }
      if (_sliceB->posZ_ > _sliceC->posZ_) { std::swap(_sliceB,_sliceC); std::swap(_Bit,_Cit); std::swap(B,C); }
      if (_sliceA->posZ_ > _sliceB->posZ_) { std::swap(_sliceA,_sliceB); std::swap(_Ait,_Bit); std::swap(A,B); }

      Vec3f b = B - A;
      Vec3f c = C - A;
      Vec3f d = C - B;
      Vec3f N = normal(_faceIter);

      for (it = _Ait ; it != _Cit && it != _slices.end() ; ++it)
      {
        Slice* _slice = const_cast<Slice*>(&(*it));

        float _ratioR = (_slice->posZ_ - A.z()) / c.z();
        Vec3f r(c.x()*_ratioR,c.y()*_ratioR,_slice->posZ_);
        Vec3f s;

        if (_slice->posZ_ - A.z() < b.z())
        {
          float _ratioS = (_slice->posZ_ - A.z()) / b.z();
          s(b.x()*_ratioS,b.y()*_ratioS,_slice->posZ_);
        } else
        {
          float _ratioS = (_slice->posZ_ - B.z()) / d.z();
          s = b + Vec3f(d.x()*_ratioS,d.y()*_ratioS,_slice->posZ_);
        }
        _slice->addSegment(A+r,A+s,N);
      }
  }
}
