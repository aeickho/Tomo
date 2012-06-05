#include <vector>

namespace tomo
{
  using std::vector;

#define N 8
/*
  struct Vec3u8
  {
    u8 x,y,z;
  };

  struct N3NodeCell
  {
    u8 d; // Position of plane
    Vec3u8 normal;
  };

  struct N3Node
  {
    N3NodeCell cell[N*N*N];
    N3Node* children; // If children == 0, then is leave
    N3Node* child(u8 x, u8 y, u8 z) { return (N3Node*)(((long long)children)+((x*N+y)*N+z)*sizeof(N3Node)); }
  };

  struct N3Tree
  {
  }

  struct Voxels 
  {
    void build(const Triangles& triangles, BoundingBox& box);
    void traverse(Ray& ray);

    N3Node* root;
    BoundingBox box;
  };
*/
}
