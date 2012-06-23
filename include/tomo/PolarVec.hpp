#ifndef _POLARVEC_HPP
#define _POLARVEC_HPP

#include "tomo/Vec.hpp"

namespace tomo
{
  template<class COORD> struct PolarVec 
  {
    typedef Vec<3,COORD> Vec;
    /// coordinate type
    typedef COORD Coord;
    static const int dimensions_ = 3;

    PolarVec() {}
    /** @brief create polar vector with angles in degrees
     * @param _longitude longitude position
     * @param _latitude latitude position
     * @param _radius radial distance
     */
    PolarVec( Coord _longitude, Coord _latitude, Coord _radius ) :
      longitude_(_longitude),
      latitude_(_latitude),
      radius_(_radius)
    {
    }
    operator Vec() const
    {
//      BOOST_STATIC_ASSERT(dimensions_==3);
      Coord _latitude = deg2rad(latitude());
      Coord _longitude = deg2rad(longitude());
      return radius()*Vec(
               sin(_latitude) * cos(_longitude),
               cos(_latitude),
               sin(_latitude) * sin(_longitude)
             );
    }
    const PolarVec& operator+=( const PolarVec& _vec )
    {
      longitude( longitude() + _vec.longitude() );
      latitude( std::min<Coord>(179.99,std::max<Coord>(0.01,latitude() + _vec.latitude())) );
      radius( std::max<Coord>(0,radius() + _vec.radius()) );
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
