#pragma once

#include "tomo/SlicableObject.hpp"
#include "tomo/LineSegment.hpp"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

namespace tomo
{
  struct MeshTraits
  {
    typedef Point3f Point;
    typedef Vec3f   Normal;
    typedef float   TexCoord1D;
    typedef Vec2f   TexCoord2D;
    typedef Vec3f   TexCoord3D;
    typedef size_t  TextureIndex;
    typedef Vec3f Color;

    VertexTraits {};
    HalfedgeTraits {};
    EdgeTraits {};
    FaceTraits {};

    VertexAttributes(OpenMesh::Attributes::Normal);
    HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);
    EdgeAttributes(OpenMesh::Attributes::Normal);
    FaceAttributes(OpenMesh::Attributes::Normal);
  };

  class Mesh : public OpenMesh::TriMesh_ArrayKernelT<MeshTraits>, public SlicableObject 
  {
    public:
      typedef enum { SM_FLAT, SM_GOURAUD } ShadeMode;

      Mesh() : shadeMode_(SM_FLAT) {}

      void read(const string& _filename);

      bool intersect(Ray3f& _ray, float& _tNear, float &_tFar, Vec3f* _normal = NULL) const;
      
      void slice(Slices& _slices) const;
      void slice(LineSegmentContainer& _lineSegmentContainer) const;

      Bounds3f bounds() const { return bounds_; }
      void calcBounds();

      TBD_PROPERTY(ShadeMode,shadeMode);
    private:
      Bounds3f bounds_;

      void sliceTriangle(ConstFaceIter _faceIter, LineSegmentContainer& _lineSegmentContainer) const; 
  };

}
