#pragma once

#include "tomo/MeshReader.hpp"

namespace tomo 
{
  class OFFReader
  {
    public:
      void read(std::string filename, Vertices* vertices = NULL, Triangles* triangles = NULL);
  };
}
