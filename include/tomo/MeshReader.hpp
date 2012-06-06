#pragma once

#include "tomo/Vertex.hpp"
#include "tomo/Triangle.hpp"

namespace tomo 
{
  class MeshReader 
  {
  public:
    virtual void read(std::string filename, 
                      vector<Vertex>* vertices = NULL, 
                      vector<Triangle>* triangles = NULL) = 0;
  };
}
