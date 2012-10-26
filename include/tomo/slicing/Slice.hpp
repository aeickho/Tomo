#pragma once

#include "tomo/geometry/prim/MultiPolygon.hpp"

namespace tomo
{
  namespace slicing
  {
    using geometry::prim::Polygon;
    using geometry::prim::MultiPolygon;

    struct Slice
    {
      Slice(float _pos);

      void add(Polygon& _polygon);
      void clear();
      void optimize(float _threshold);

      TBD_PROPERTY_RO(float,pos)
      TBD_PROPERTY_REF(MultiPolygon,polygons)
    };

  }
}
