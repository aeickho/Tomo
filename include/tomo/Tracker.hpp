#pragma once
#include <algorithm>

namespace tomo
{
  template<class POINT,class VEC> struct Tracker
  {
    /// point type
    typedef POINT Point;
    /// vector type
    typedef VEC Vec;
    
    enum Mode
    {
      ORTHOGONAL,
      SPHERICAL
    };

    Tracker()
    {
    }
    Tracker( const Tracker& _tracker ) 
    {
      center_ = _tracker.center_;
      direction_ = _tracker.direction_;
    }
    Tracker( const Point& _center, const Vec& _direction ) :
      center_(_center),
      direction_(_direction)
    {
    }
    void track( int x, int y, int z, Mode _mode = SPHERICAL )
    {
      switch( _mode )
      {
        case ORTHOGONAL:
          /// @todo move orthogonal to direction()
          BOOST_ASSERT(0);
          break;
        case SPHERICAL:
          direction_.sphericalMove(x,y,z);
          break;
      }
    }
    /// get tracker position
    Point eye() const
    {
      return center_ + direction_;
    }
    void eye( const Point& _pos )
    {
      direction_ = _pos - center_;
    }
    /// target position
    TBD_PROPERTY_REF(Point,center);
    /// tracker's postion relatively to target
    TBD_PROPERTY_REF(Vec,direction);
  };
}
