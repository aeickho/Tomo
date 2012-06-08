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
      float recKDTreeTraverse(Ray& ray, Node* node, float tnear, float tfar, bool& found) const;

    private:
      void divideNode(Node* node, const BoundingBox& box, int depth);
  };

  struct Mesh : public SceneObject
  {
      void read(string filename);

      bool intersect(Ray& ray) const;
      void draw(Color color = Color()) const;

      std::pair<Mesh,Mesh> split(const Plane& plane);
      
      TexCoords texCoords(const Ray& ray) const { return TexCoords(); }
      Vec3f normal(const Ray& ray) const { return Vec3f(); }

      vector<Triangle> triangles;
      
      void displayNormals();
    protected:
      void calcNormals();
      void calcBoundingBox();
    private:

      void splitTriangle(const Triangle& tri, const Plane& plane, Mesh& behind, Mesh& front);
      TriangleKDTree kdTree;
      GLuint dispList;
      // DisplayList
  };

}
