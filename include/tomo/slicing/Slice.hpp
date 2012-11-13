#pragma once

#include "tomo/geometry/prim/MultiPolygon.hpp"

namespace tomo
{
  namespace slicing
  {
    struct Slice
    {
      typedef geometry::prim::MultiPolygon MultiPolygon;
      typedef geometry::prim::Polygon Polygon;

      Slice(float _pos);

      void add(Polygon& _polygon);
      void clear();
      void optimize(float _threshold);

      TBD_PROPERTY_RO(float,pos)
      TBD_PROPERTY_REF(MultiPolygon,polygons)
    };

  }
}
