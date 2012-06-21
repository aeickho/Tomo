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
    typedef typename Point::CoordType CoordType;

    Camera() {}
    Camera( const Tracker& _tracker, CoordType _near, CoordType _far, const Point& _up ) :
      Tracker(_tracker),
      up_(_up),
      near_(_near),
      far_(_far)
    {
    }

    /// camera orientation
    TBD_PROPERTY_REF(Point,up);
    /// near plane
    TBD_PROPERTY(CoordType,near);
    /// far plane
    TBD_PROPERTY(CoordType,far);
  };
}
