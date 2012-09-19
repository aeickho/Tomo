#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <vector>
#include <tbd/property.h>
#include "tomo/slicing/PlaneStack.hpp"
#include "tomo/geometry/prim/Polygon.hpp"
#include <boost/foreach.hpp>
#include <set>
#include <list>


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

    struct Slices : PlaneStack<float,Slice>
    {
    public:
      Slices(float _thickness, geometry::aux::Bounds3f _bounds);

      void build(float _thickness, geometry::aux::Bounds3f _bounds);
    };
  }
}
