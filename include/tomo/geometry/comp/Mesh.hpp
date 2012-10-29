#pragma once

#include "tomo/geometry/prim/Primitive.hpp"
#include "tomo/geometry/base/Point.hpp"
#include "tomo/slicing/SegmentStack.hpp"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>


namespace tomo
{
  namespace geometry
  {
    namespace comp
    {
      struct MeshTraits
      {
        typedef geometry::base::Point3f Point3f;
        typedef geometry::base::Vec3f Vec3f;
        typedef geometry::base::Vec2f Vec2f;

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

      class Mesh : public OpenMesh::TriMesh_ArrayKernelT<MeshTraits>,
        public prim::Primitive3f
      {
      public:
        typedef enum { SM_FLAT, SM_GOURAUD } ShadeMode;

        Mesh() : shadeMode_(SM_FLAT) {}
        Mesh(const std::string& _filename) :
          shadeMode_(SM_FLAT)
        {
          read(_filename);
        }

        void read(const std::string& _filename)
        {
          TOMO_ASSERT(OpenMesh::IO::read_mesh(*this, _filename));
          update();
        }

        void update()
        {
          update_face_normals();
          update_vertex_normals();
          calcBounds();
        }

        TBD_PROPERTY(ShadeMode,shadeMode);
        TBD_PROPERTY(bounds_type,bounds);
      private:
        void calcBounds()
        {
          bounds_type _bounds;
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
      };
    }
  }
}
