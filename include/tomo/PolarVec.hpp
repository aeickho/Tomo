#ifndef _POLARVEC_HPP
#define _POLARVEC_HPP

#include "tomo/Vec.hpp"
#include "tbd/log.h"

namespace tomo
{
  template<class COORD> struct PolarVec 
  {
    typedef tomo::Vec<3,COORD> Vec;
    typedef tomo::Vec<4,COORD> Vec4;
    /// coordinate type
    typedef COORD Coord;
    static const int dimensions_ = 3;

    PolarVec() {}
    PolarVec( Coord _longitude, Coord _latitude, Coord _radius ) :
      longitude_(_longitude),
      latitude_(_latitude),
      radius_(_radius)
    {
    }
    operator Vec() const
    {
      BOOST_STATIC_ASSERT(dimensions_==3);
      Coord _latitude = deg2rad(latitude());
      Coord _longitude = deg2rad(longitude());
      return radius()*Vec(
               sin(_latitude) * cos(_longitude),
               cos(_latitude),
               sin(_latitude) * sin(_longitude)
             );
    }
    operator Vec4() const
    {
      return Vec4(*this,1.0);
    }
    const PolarVec& operator+=( const PolarVec& _vec )
    {
      longitude( longitude() + _vec.longitude() );
      latitude( std::min<Coord>(179.99,std::max<Coord>(0.01,latitude() + _vec.latitude())) );
      radius( std::max<Coord>(0,radius() + _vec.radius()) );
//      LOG_MSG << fmt("(long,lat,radius) %,%,%") % longitude() % latitude() % radius();
      return *this;
    }

    TBD_PROPERTY(Coord,longitude);
    TBD_PROPERTY(Coord,latitude);
    TBD_PROPERTY(Coord,radius);

  protected:
    static inline Coord deg2rad( Coord _value )
    {
      return _value * M_PI / 180.0;
    }
    static inline Coord rad2deg( Coord _value )
    {
      return _value * 180.0 / M_PI;
    }
  };
}

#endif /* _POLARVEC_HPP */
