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
      float recKDTreeTraverse(Ray& ray, Node* node, float tnear, float tfar, bool& found);


    private:
      void divideNode(Node* node, const BoundingBox& box, int depth);
  };
  struct Mesh : public SceneObject
  {
      void read(string filename);

      bool intersect(Ray& ray);
      void draw(Color color = Color());

      std::pair<TriangleMesh,TriangleMesh> split(const Plane& splitPlane);
      
      TexCoords texCoords(const Ray& ray) { return TexCoords(); }
      Vec3f normal(const Ray& ray) { return Vec3f(); }

      vector<Triangle> triangles;
      
      void displayNormals();
    protected:
      void calcNormals();
      void calcBoundingBox();
    private:

      void splitTriangle(const Triangle& tri, const Plane& plane, TriangleMesh& behind, TriangleMesh& front);
      TriangleKDTree kdTree;
      GLuint dispList;
      // DisplayList
  };

}
