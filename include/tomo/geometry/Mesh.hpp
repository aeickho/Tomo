#pragma once

#include "tomo/slicing/LineSegmentPlane.hpp"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

namespace tomo
{
  namespace geometry
  {
    struct MeshTraits
    {
      typedef base::Point3f Point3f;
      typedef base::Vec3f Vec3f;
      typedef base::Vec2f Vec2f;

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

    class Mesh : public OpenMesh::TriMesh_ArrayKernelT<MeshTraits>
    {
    public:
      typedef enum { SM_FLAT, SM_GOURAUD } ShadeMode;

      Mesh() : shadeMode_(SM_FLAT) {}

      void read(const string& _filename);

      void slice(slicing::Slices& _slices) const;
      void slice(slicing::LineSegmentContainer& _lineSegmentContainer) const;

      void calcBounds();

      TBD_PROPERTY(ShadeMode,shadeMode);
      TBD_PROPERTY_RO(aux::Bounds3f,bounds);
    private:

      void sliceTriangle(ConstFaceIter _faceIter,
                         slicing::LineSegmentContainer& _lineSegmentContainer) const;
    };
  }
}
