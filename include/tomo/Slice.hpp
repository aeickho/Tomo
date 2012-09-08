#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <vector>
#include <tbd/property.h>
#include "tomo/PlaneStack.hpp"
#include "tomo/Bounds.hpp"
#include <boost/foreach.hpp>
#include <set>
#include <list>


namespace tomo
{
 typedef boost::geometry::model::d2::point_xy<float> PointXYf;
  typedef boost::geometry::model::polygon< PointXYf > Polygon;

  using std::vector;

  struct Slice
  {
    Slice(float _pos);
    
    void add(Polygon& _polygon);
    void clear();
    void optimize(float _threshold);

    TBD_PROPERTY_RO(float,pos)
    TBD_PROPERTY_REF(std::vector<Polygon>,polygons)
  };

  struct Slices : PlaneStack<float,Slice>
  {
  public:
      Slices(float _thickness, Bounds3f _bounds);

      void build(float _thickness, Bounds3f _bounds);
  };

}
