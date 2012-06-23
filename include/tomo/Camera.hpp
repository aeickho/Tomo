#pragma once
#include "tbd/property.h"
#include <boost/assert.hpp>

namespace tomo
{
  template<class TRACKER> struct Camera : TRACKER 
  {
    /// tracker type
    typedef TRACKER Tracker;
    /// point type from tracker
    typedef typename Tracker::Point Point;    
    /// coordinate type from tracker
    typedef typename Point::Coord Coord;

    Camera() {}
    Camera( const Tracker& _tracker, Coord _near, Coord _far, const Point& _up ) :
      Tracker(_tracker),
      up_(_up),
      near_(_near),
      far_(_far)
    {
    }

    /// camera orientation
    TBD_PROPERTY_REF(Point,up);
    /// near plane
    TBD_PROPERTY(Coord,near);
    /// far plane
    TBD_PROPERTY(Coord,far);
  };
}
