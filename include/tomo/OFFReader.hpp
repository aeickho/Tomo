#pragma once

#include "tomo/MeshReader.hpp"

namespace tomo 
{
  class OFFReader
  {
    public:
      void read(std::string filename, vector<Vertex>* vertices = NULL, vector<Triangle>* triangles = NULL);
  };
}
