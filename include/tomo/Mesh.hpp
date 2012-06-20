#pragma once

#include "tomo/SceneObject.hpp"
#include "tomo/Vertex.hpp"
#include "tomo/Triangle.hpp"
#include "tomo/KDTree.hpp"

namespace tomo
{ 
  class TriangleKDTree : public KDTree<Triangle>
  {
    public:
      TriangleKDTree();
      
      // Traverses kd-tree along a ray recursively
      float recKDTreeTraverse(Ray& ray, Node* node, float tnear, float tfar, bool& found) const;

    private:
      // Method to define how KDTree is constructed 
      void divideNode(Node* node, const BoundingBox& box, int depth);
  };

  template<class TRIANGLE>
  struct MeshConcept : public Compound<TRIANGLE>
  {
      virtual void read(const string& filename) = 0;
      
      bool intersect(Ray& _ray, Vec3f* _normal = NULL, TexCoords* _texCoords = NULL) const;
    
      void displayNormals();
      
      std::vector<Triangle>& triangles() { return triangles_; }
      const std::vector<Triangle>& triangles() const { return triangles_; }
    
    protected:
      void calcBoundingBox();
    private:
      std::vector<TRIANGLE> triangles_;
  };

  struct VertexMesh : public Compound<VertexTriangle>
  {

  };

  struct TriangleMesh : public Compound<Triangle>
  {

      // Split a mesh into halves along a split plane
      std::pair<Mesh,Mesh> split(const Plane& plane);
      
     protected:

      // Iterate over triangles and determine maximum  
    private:

      // Splits are triangle with splitting plane
      // Adds triangles behind plane to behind and triangles in front of plane to front 
      void splitTriangle(const Triangle& tri, const Plane& plane, Mesh& behind, Mesh& front);
  };

}
