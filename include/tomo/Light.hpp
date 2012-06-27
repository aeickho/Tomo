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
    /// coordinate type from tracker
    typedef typename Tracker::Point Point;
    typedef typename Tracker::Point::Coord Coord;
    /// point type from tracker
    typedef tomo::Point<4,Coord> Point4;

    Light() {}
    Light( const Tracker& _tracker,
        const Color& _ambient, 
        const Color& _diffuse, 
        const Color& _specular, 
        const Coord _intensity, 
        Coord _shadows, 
        Coord _radius,
        bool _positional=true) :
      Tracker(_tracker),
      ambient_(_ambient),
      diffuse_(_diffuse),
      specular_(_specular),
      intensity_(_intensity),
      shadows_(_shadows),
      radius_(_radius),
      positional_(_positional)
    {
    }
    Point4 eye4() const
    {
      Point e=Tracker::eye();
      return Point4(e.x(),e.y(),e.z(),positional_?1.0:0.0);
    }

    TBD_PROPERTY_REF(Color,ambient);
    TBD_PROPERTY_REF(Color,diffuse);
    TBD_PROPERTY_REF(Color,specular);
    TBD_PROPERTY(Coord,intensity);
    TBD_PROPERTY(Coord,shadows);
    TBD_PROPERTY(Coord,radius);
    TBD_PROPERTY(bool,positional);
  };
}

