#pragma once

#include "Polygon.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      typedef std::vector<BoostPolygon> BoostMultiPolygon;
      
      struct MultiPolygon : Primitive2f
      {
        typedef std::vector<Ring> Rings;

        MultiPolygon();
        MultiPolygon(const Rings& _rings);
        MultiPolygon(const BoostMultiPolygon& _boostMultiPolygon);

        void from(const Rings& _rings);
        void from(const BoostMultiPolygon& _boostMultiPolygon);

        void fetchLineSegments(std::vector<LineSegment>& _lineSegments) const;
        void fetch(BoostMultiPolygon& _boostMultiPolygon) const;
      
        TBD_PROPERTY_REF(std::vector<Polygon>,polygons);

      private:
        void unify(const Rings& _input, Rings& _output) const;
      };
    }
  }
}
