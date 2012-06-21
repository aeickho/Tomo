#pragma once 
#include "tomo/Vector.hpp"
#include <vector>

namespace tomo
{
  template<class TRACKER, class COLOR> struct Light : TRACKER 
  {
    /// tracker type
    typedef TRACKER Tracker;
    typedef COLOR Color;
    /// point type from tracker
    typedef typename Tracker::Point Point;    
    /// coordinate type from tracker
    typedef typename Point::Coord Coord;

    void setup( 
        const Point& _pos, 
        const Color& _ambient, const Color& _diffuse, const Color& _specular, 
        const Coord _intensity, Coord _shadows, Coord _radius )
    {
      pos_ = _pos;
      ambient_ = _ambient;
      specular_ = _specular;
      intensity_ = _intensity;
      shadows_ = _shadows;
      radius_ = _radius;
    }


    TBD_PROPERTY_REF(Point,pos);
    TBD_PROPERTY_REF(Color,ambient);
    TBD_PROPERTY_REF(Color,diffuse);
    TBD_PROPERTY_REF(Color,specular);
    TBD_PROPERTY(Coord,intensity);
    TBD_PROPERTY(Coord,shadows);
    TBD_PROPERTY(Coord,radius);
  };
}

