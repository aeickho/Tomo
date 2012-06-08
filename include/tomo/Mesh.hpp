#pragma once

#include "tomo/SceneObject.hpp"
#include "tomo/Vertex.hpp"
#include "tomo/Triangle.hpp"
#include "tomo/KDTree.hpp"

#include <GL/glu.h>

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

  struct Mesh : public SceneObject
  {
      void read(string filename);

      // Test if mesh intersects ray.
      // @detail  Uses TriangleKDTree for fast traversal
      bool intersect(Ray& ray) const;

      // Draws Mesh with OpenGL
      // TODO: Draw with display list
      void draw(Color color = Color()) const;

      // Split a mesh into halves along a split plane
      std::pair<Mesh,Mesh> split(const Plane& plane);
   
      TexCoords texCoords(const Ray& ray) const { return TexCoords(); }
      Vec3f normal(const Ray& ray) const { return Vec3f(); }
   
      void displayNormals();
    protected:

      // Iterate over triangles and determine maximum  
      void calcBoundingBox();
    private:

      // Splits are triangle with splitting plane
      // Adds triangles behind plane to behind and triangles in front of plane to front 
      void splitTriangle(const Triangle& tri, const Plane& plane, Mesh& behind, Mesh& front);
      TriangleKDTree kdTree;

      vector<Triangle> triangles;
      GLuint dispList;
      // DisplayList
  };

}
