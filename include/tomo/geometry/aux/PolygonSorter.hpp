#pragma once

#include "tomo/geometry/prim/Polygon.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      class PolygonSorter 
      {
        public:
          typedef std::vector<prim::Ring> Rings;
          
          PolygonSorter() {}
          PolygonSorter(const Rings& _rings, prim::MultiPolygon& _polygons);

          void sort(const Rings& _rings, prim::MultiPolygon& _polygons) const;
        private:
          
      };
    }
  }
}
