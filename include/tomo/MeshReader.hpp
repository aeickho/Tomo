#pragma once

#include "tomo/Vertex.hpp"
#include "tomo/Triangle.hpp"

namespace tomo 
{
  class MeshReader 
  {
  public:
    virtual void read(std::string filename, Vertices* vertices = NULL, Triangles* triangles = NULL) = 0;
  };
}
