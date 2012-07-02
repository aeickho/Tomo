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
    typedef typename Tracker::Vec Vec;    
    typedef typename Tracker::PolarVec PolarVec;    
    /// coordinate type from tracker
    typedef typename Vec::Coord Coord;

    Camera() {}
    Camera( const Tracker& _tracker, Coord _near, Coord _far, const Vec& _up ) :
      Tracker(_tracker),
      up_(_up),
      near_(_near),
      far_(_far)
    {
    }

    Vec right() const
    {
      return cross((Vec)Tracker::direction(),up_);
    }

    void strafe( Coord _right )
    {
      Tracker::center( Tracker::center() + right().normalized() * _right );
    }
    void lift( Coord _up )
    {
      Vec ortho_up = cross(right(),Tracker::direction());
      Tracker::center( Tracker::center() + ortho_up.normalized() * _up );
    }

    /// camera orientation
    TBD_PROPERTY_REF(Vec,up);
    /// near plane
    TBD_PROPERTY(Coord,near);
    /// far plane
    TBD_PROPERTY(Coord,far);
  };
}
