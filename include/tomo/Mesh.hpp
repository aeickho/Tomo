#pragma once

#include "tomo/SceneObject.hpp"
#include "tomo/Vertex.hpp"

namespace tomo
{
  struct Mesh : public SceneObject
  {
      virtual void read(string filename) = 0;

      Vertices vertices;

      void displayNormals();
    protected:
      void calcBoundingBox();
  };
}
