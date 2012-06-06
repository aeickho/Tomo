#pragma once

#include "tomo/Triangle.hpp"
#include "tomo/KDTree.hpp"
#include "tomo/Mesh.hpp"


#ifdef __APPLE__ 
#include <GLUT/glu.h>
#else
#include <GL/glu.h>
#endif


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

  struct TriangleMesh : public Mesh
  {
      void read(string filename);

      bool intersect(Ray& ray);
      void draw(Color color = Color());

      TexCoords texCoords(const Ray& ray) { return TexCoords(); }
      Vec3f normal(const Ray& ray) { return Vec3f(); }

      vector<Triangle> triangles;
    protected:
      void calcTriangles();
      void calcNormals();

    private:

      TriangleKDTree kdTree;
      GLuint dispList;
      // DisplayList
  };
}
