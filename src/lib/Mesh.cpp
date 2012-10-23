#include "tomo/geometry/comp/Mesh.hpp"
#include <boost/foreach.hpp>
#include <tbd/log.h>

using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace comp
    {
      using namespace base;

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

      void Mesh::slice(slicing::Slices& _slices) const
      {
#define NUM_THREADS 1
// TODO: Parallelize this!
//    omp_set_num_threads(NUM_THREADS);
        slicing::SegmentStack* _segmentStack[NUM_THREADS];
//   #pragma omp parallel for
        for (int i = 0; i < NUM_THREADS; i++)
          _segmentStack[i] = new slicing::SegmentStack(_slices);
        Mesh::ConstFaceIter    fIt(faces_begin()),fEnd(faces_end());
//    #pragma omp parallel for
        for (; fIt!=fEnd; ++fIt)
          sliceTriangle(fIt,*_segmentStack[0]);
        for (int i = 1; i < NUM_THREADS; i++)
        {
          _segmentStack[0]->aggregate(*_segmentStack[i]);
        }
        _segmentStack[0]->produceSlices();
        for (int i = 0; i < NUM_THREADS; i++)
          delete _segmentStack[i];
      }

      void Mesh::slice(slicing::SegmentStack& _segmentStack) const
      {
        Mesh::ConstFaceIter    fIt(faces_begin()),fEnd(faces_end());
//    #pragma omp parallel for
        for (; fIt!=fEnd; ++fIt)
          sliceTriangle(fIt,_segmentStack);
      }
      void Mesh::sliceTriangle(ConstFaceIter _faceIter,
                               slicing::SegmentStack& _segmentStack) const
      {
        using prim::Segment;

        Mesh::ConstFaceVertexIter fvIt = cfv_iter(_faceIter.handle());
        Point3f A = point(fvIt), B = point(++fvIt), C = point(++fvIt);
        slicing::SegmentStack::iterator _Ait = _segmentStack.get(A.z()),
                                        _Bit = _segmentStack.get(B.z()),
                                        _Cit = _segmentStack.get(C.z()),
                                        it;

        typedef Compound<Segment> SegmentPlane;
        SegmentPlane* _segmentPlaneA = &(_Ait->second);
        SegmentPlane* _segmentPlaneB = &(_Bit->second);
        SegmentPlane* _segmentPlaneC = &(_Cit->second);
        // If all vertices lay in the same lineSegmentPlane, add a triangle
        // XXX: Skip this for now...
        /*if ((_Ait == _Bit) && (_Bit == _Cit))
        {
          _lineSegmentPlaneA->addSegment(A,B);
          _lineSegmentPlaneA->addSegment(A,C);
          _lineSegmentPlaneA->addSegment(B,C);
          return;
        }*/
        /// Sort vertices and corresponding lineSegmentPlane iterators,
        /// So that A is the lower, B in the middle and C the upper vertex
        ///       /C
        ///      / |
        ///      B |
        ///      \ |
        ///       \A
        if (_Ait->first > _Bit->first)
        {
          std::swap(_segmentPlaneA,_segmentPlaneB);
          std::swap(_Ait,_Bit);
          std::swap(A,B);
        }
        if (_Bit->first > _Cit->first)
        {
          std::swap(_segmentPlaneB,_segmentPlaneC);
          std::swap(_Bit,_Cit);
          std::swap(B,C);
        }
        if (_Ait->first > _Bit->first)
        {
          std::swap(_segmentPlaneA,_segmentPlaneB);
          std::swap(_Ait,_Bit);
          std::swap(A,B);
        }
        Point2f _Aproj = A.project(Z);
        Vec3f b = B - A;
        scalar_type _invBz = 1.0 / b.z();
        Vec3f c = C - A;
        scalar_type _invCz = 1.0 / c.z();
        Vec3f d = C - B;
        scalar_type _invDz = 1.0 / d.z();
        Vec3f N = normal(_faceIter);
        // TODO Extend this to be flexible with slicing axis (only Z axis is supported for now)
        scalar_type _ratioR, _ratioS;
        Vec2f r,s;
        for (it = _Ait ; it != _Bit && it != _segmentStack.end(); ++it)
        {
          scalar_type _pos = it->first - A.z();
          SegmentPlane* _segmentPlane = &(it->second);
          _ratioR = _pos * _invCz;
          _ratioS = _pos * _invBz;
          r(c.x()*_ratioR,c.y()*_ratioR);
          s(b.x()*_ratioS,b.y()*_ratioS);
          Point2f _p0 = _Aproj+r, _p1 = _Aproj+s;
          Vec2f _normal = _p1 - _p0;  // Normal of segment points
          if (_normal.sqrLength() == 0.0) continue;
          _normal(-_normal[1],_normal[0]);
          /// Swap point to assure that points are in proper order to be able to make lineSegments
          if (dot(Vec2f(N.x(),N.y()),_normal) > 0.0) std::swap(_p0,_p1);
          _segmentPlane->add(Segment(_p0,_p1));
        }
        for (it = _Bit ; it != _Cit && it != _segmentStack.end(); ++it)
        {
          scalar_type _pos = it->first;
          SegmentPlane* _segmentPlane = &(it->second);
          _ratioR = (_pos - A.z()) * _invCz;
          _ratioS = (_pos - B.z()) * _invDz;
          r(c.x()*_ratioR,c.y()*_ratioR);
          s(b.x() + d.x()*_ratioS,b.y() + d.y()*_ratioS);
          Point2f _p0 = _Aproj+r, _p1 = _Aproj+s;
          Vec2f _normal = _p1 - _p0;  // Normal of segment points
          if (_normal.sqrLength() == 0.0) continue;
          _normal(-_normal[1],_normal[0]);
          /// Swap point to assure that points are in proper order to be able to make lineSegments
          if (dot(Vec2f(N.x(),N.y()),_normal) > 0.0) std::swap(_p0,_p1);
          _segmentPlane->add(Segment(_p0,_p1));
        }
      }
    }
  }
}
