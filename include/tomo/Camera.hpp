#pragma once 
#include "tomo/Ray.hpp"
#include <math.h>
#include <algorithm>

namespace tomo 
{
  template<class COORD> struct Camera
  {
    typedef COORD Coord;
    typedef Coord Angle;
    typedef tomo::Point<3,Coord> Point;

    bool valid() const { return valid_; }

    const Point& center() const { return center_; }
    void center( const Point& _center ) 
    { 
      center_ = _center; 
      valid_ = false; 
    }

    Angle latitude() const { return latitude_; }
    void latitude( Angle _latitude ) 
    {
      if( _latitude > 90 )
        _latitude = 90;
      else if( _latitude < -90 )
        _latitude = -90;
      else
        latitude_ = _latitude; 
      valid_ = false; 
    }

    Angle longitude() const { return longitude_; }
    void longitude( Angle _longitude ) 
    { 
      longitude_ = _longitude; 
      valid_ = false; 
    }

    void move( Angle _longitude, Angle _latitude )
     {
      longitude( longitude() + _longitude );
      latitude( latitude() + _latitude );
    }

    Coord distance() const { return distance_; }
    void distance( Coord _distance ) 
    { 
      distance_ = std::max(_distance,(Coord)0); 
      valid_ = false; 
    }

    const Point& eye() { return eye_; }
    const Point& up() { return up_; }

    Camera() :
      valid_(false),
      center_(0,0,0),
      latitude_(0),
      longitude_(0),
      distance_(0)
    {
    } 
    void update()
    {
      if( !valid_ ) 
      {
#define PI 3.14
        /// @todo optimize: move the division to getter/setter of meridian and latiude?!
        eye_.x( center_.x() + distance_ * cos( latitude_ / 180.0 * PI ) * cos( longitude_ / 180.0 * PI ) );
        eye_.y( center_.y() + distance_ * sin( latitude_ / 180.0 * PI ) );
        eye_.z( center_.z() + distance_ * cos( latitude_ / 180.0 * PI ) * sin( longitude_ / 180.0 * PI ) ); 
        up_.x(0);
        up_.y(1);
        up_.z(0);
        valid_ = true;
      }
    }

    protected:
    /// calculations are done
    bool valid_;
    /// point of interest
    Point center_;
    /// latitude of the camera postition on a sphere arround center
    Angle latitude_;
    /// longitude/meridian of the camera position on a sphere arround center
    Angle longitude_;
    /// distance of the camera from center
    Coord distance_;
    /// camera position
    Point eye_;
    /// camera orientation
    Point up_;
  };
}
