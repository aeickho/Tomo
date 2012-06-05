#pragma once

#include "tomo/Vertex.hpp"

namespace tomo 
{
  class MeshWriter
  {
  public:
    virtual void write(std::string filename, Vertices* vertices = NULL, Polygons* polygons = NULL) = 0;
  };
}
