
#include "tomo/geometry/comp/Mesh.hpp"
#include "tomo/slicing/SegmentStack.hpp"
#include "tomo/misc.hpp"

namespace tomo
{
  namespace slicing
  {
    struct TriangleSlicer
    {
      typedef geometry::comp::Mesh Mesh;
      typedef geometry::base::Matrix4f Matrix4f;

      void operator()(const Mesh& _mesh, const Matrix4f& _matrix, SegmentStack& _segmentStack);
      {
        TOMO_NOT_TESTED();

        Mesh::ConstFaceIter    fIt(faces_begin()),fEnd(faces_end());
        for (; fIt!=fEnd; ++fIt)
        {
          Mesh::ConstFaceVertexIter fvIt = cfv_iter(_faceIter.handle());
          Point3f A = _matrix*point(fvIt),
                  B = _matrix*point(++fvIt),
                  C = _matrix*point(++fvIt);
          // Sort vertices and corresponding lineSegmentPlane iterators,
          // So that A is the lower, B in the middle and C the upper vertex
          //       /C
          //      / |
          //      B |
          //      \ |
          //       \A
          if (A.z() > B.z()) std::swap(A,B);
          if (B.z() > C.z()) std::swap(B,C);
          if (A.z() > B.z()) std::swap(A,B);

          sliceTriangle(A,B,C,_segmentStack);
        }
      }

    private:
      void sliceTriangle(const Point3f& A,
                         const Point3f& B,
                         const Point3f& C,
                         SegmentStack& _segmentStack) const
      {
        TOMO_NOT_TESTED();
        using prim::Segment;

        slicing::SegmentStack::iterator _Ait = _segmentStack.get(A.z()),
                                        _Bit = _segmentStack.get(B.z()),
                                        _Cit = _segmentStack.get(C.z()),
                                        it;

        typedef Compound<Segment> SegmentPlane;
        SegmentPlane* _segmentPlaneA = &(_Ait->second);
        SegmentPlane* _segmentPlaneB = &(_Bit->second);
        SegmentPlane* _segmentPlaneC = &(_Cit->second);

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
    };
  }
}
