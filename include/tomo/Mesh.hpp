#pragma once

#include "tomo/SlicableObject.hpp"
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

      /** @brief Split a mesh into halves along a split plane
       * @param _plane  Split plane
       * @returns Two new TriangleMeshes which represent halves of the original mesh 
       */
      std::pair<Mesh,Mesh> split(const Plane& plane);

      bool intersect(Ray3f& _ray, float& _tNear, float &_tFar, Vec3f* _normal = NULL) const;

      bool slice(Slice& _slice) const 
      { 
        return false; // TODO: Implement slicing algo
      }

      void slice(Slices& _slices) const;
      Bounds3f bounds() const { return bounds_; }
      void calcBounds();

      TBD_PROPERTY(ShadeMode,shadeMode);
    private:
      Bounds3f bounds_;

      /** @brief Splits are triangle with splitting plane
       * @detail Adds triangles behind plane to behind and triangles in front of plane to front
       * @param _tri    Triangle to be split
       * @param _plane  Split plane
       * @param _halves Halves of the mesh where the resulting triangles are inserted 
       */
      void splitTriangle(ConstFaceIter _faceIter, const Plane& plane, std::pair<Mesh,Mesh>& _halves);
      void sliceTriangle(ConstFaceIter _faceIter, Slices& _slices) const; 
  };

}
