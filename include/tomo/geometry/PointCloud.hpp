#pragma once

#include "tomo/geometry/prim/Vertex.hpp"
#include "tomo/geometry/aux/Compound.hpp"

#include <map>

namespace tomo
{
  namespace geometry
  {
  class PointCloud : public aux::Compound<prim::Vertex3f,3,float>
  {
    public:
      PointCloud();

      void read(const string& _filename);
      bool intersect(ray_type& _ray, scalar_type& _tNear, scalar_type&_tFar, vector_type* _normal = NULL) const { return false; }

    private:
  };
  }
}
