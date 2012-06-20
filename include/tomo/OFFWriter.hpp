#pragma once

#include "tomo/MeshWriter.hpp"

namespace tomo 
{
  class OFFWriter
  {
    public:
      void write(std::string filename, const Vertices* vertices = NULL, const Polygons* polygons = NULL) const;
  };
}
