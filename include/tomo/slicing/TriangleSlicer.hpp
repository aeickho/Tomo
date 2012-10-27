
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
      typedef Mesh::scalar_type scalar_type;
      typedef Mesh::point_type point_type;
      typedef Mesh::point_type::matrix_type matrix_type;
      typedef Mesh::vec_type vec_type;
      
      typedef geometry::base::Point2f Point2f;
      typedef geometry::base::Vec2f Vec2f;

      void operator()(const Mesh& _mesh, const matrix_type& _matrix, SegmentStack& _segmentStack)
      {
        TOMO_NOT_TESTED();

        Mesh::ConstFaceIter    fIt(_mesh.faces_begin()),fEnd(_mesh.faces_end());
        for (; fIt!=fEnd; ++fIt)
        {
          Mesh::ConstFaceVertexIter fvIt = _mesh.cfv_iter(fIt.handle());
          vec_type N = _mesh.normal(fIt);
          point_type A = _matrix*_mesh.point(fvIt),
                     B = _matrix*_mesh.point(++fvIt),
                     C = _matrix*_mesh.point(++fvIt);
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

          sliceTriangle(A,B,C,N,_segmentStack);
        }
      }

    private:
      void sliceTriangle(const point_type& A,
                         const point_type& B,
                         const point_type& C,
                         const vec_type& N,
                         SegmentStack& _segmentStack) const
      {
        TOMO_NOT_TESTED();
        using geometry::prim::Segment;
        typedef geometry::comp::Compound<Segment> SegmentPlane;
        
        SegmentStack::iterator _Ait = _segmentStack.get(A.z()),
                               _Bit = _segmentStack.get(B.z()),
                               _Cit = _segmentStack.get(C.z()),
                               it;

        Point2f _Aproj = A.project(geometry::base::Z);
        vec_type b = B - A;
        scalar_type _invBz = 1.0 / b.z();
        vec_type c = C - A;
        scalar_type _invCz = 1.0 / c.z();
        vec_type d = C - B;
        scalar_type _invDz = 1.0 / d.z();
        scalar_type _ratioR, _ratioS, _pos;
        Vec2f r,s;

        for (it = _Ait ; it != _Cit && it != _segmentStack.end(); ++it)
        {
          SegmentPlane* _segmentPlane = &(it->second);
          
          if (it->first < _Bit->first)
          {
            _pos = it->first - A.z();
            _ratioR = _pos * _invCz;
            _ratioS = _pos * _invBz;
            s(b.x()*_ratioS,b.y()*_ratioS);
          } else
          {
            _pos = it->first;
            _ratioR = (_pos - A.z()) * _invCz;
            _ratioS = (_pos - B.z()) * _invDz;
            s(b.x() + d.x()*_ratioS,b.y() + d.y()*_ratioS);
          }
          
          r(c.x()*_ratioR,c.y()*_ratioR);

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
