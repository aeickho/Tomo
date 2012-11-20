#pragma once

#include "circle.hpp"
#include "tomo/geometry/prim/Polygon.hpp"

namespace tomo
{
  namespace test
  {
    namespace generators
    {
      namespace create
      {
        /// create demo circle
        geometry::prim::Polygon circleWith3Holes(
            geometry::prim::Polygon::point_type _center, 
            float _radius,            
            int _numSegments = 48)
        {
          typedef geometry::prim::Polygon Polygon;
          typedef Polygon::point_type point_type;
          
          Polygon _polygon;
          _polygon.boundary(circle(_center,_radius,false,_numSegments));

          _polygon.add(circle(_center + point_type(-_radius/3,-_radius/3),_radius/4,true,_numSegments));
          _polygon.add(circle(_center + point_type(_radius/3,-_radius/3),_radius/4,true,_numSegments));
          _polygon.add(circle(_center + point_type(0,_radius/3),_radius/4,true,_numSegments));

          _polygon.update();

          return _polygon;
        }
      }
    }
  }
}

