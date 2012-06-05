#pragma once

#include "tomo/MeshWriter.hpp"

namespace tomo 
{
  class OFFWriter
  {
    public:
      void write(std::string filename, Vertices* vertices = NULL, Polygons* polygons = NULL);
  };
}
