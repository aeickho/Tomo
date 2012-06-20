#pragma once

//#include <boost/polygon/polygon.hpp>

#include "tomo/Ray.hpp"

namespace tomo
{
  typedef Polygon 

  /*
  typedef boost::polygon::point_data<int> Point2D;
  typedef boost::polygon::polygon_set_data<int> PolygonSet;
  typedef boost::polygon::polygon_with_holes_data<int> Polygon;*/
  typedef std::pair<point, point> Edge;
  //using namespace boost::polygon::operators;

  typedef std::vector<Point2i> Polygon;

  typedef enum { NONE, SURFACE = 0x01, SUPPORT = 0x02, FILL = 0x04, ALL = 0x07 } IsleType;

  struct Isle
  {
  Polyline border_;
    std::vector<Polyline> holes_;
    std::vector<Polyline> fill_, support_;	  
    
    virtual IsleType type() { return NONE; }
  };


  struct SurfaceIsle : public Isle
  {
    IsleType type() { return SURFACE; }
  };

  struct SupportIsle : public Isle
  {
    
    IsleType type() { return SUPPORT; }
  };

  struct FillIsle : public Isle
  {
    IsleType type() { return FILL; }
  };
 

}
