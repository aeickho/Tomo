#ifndef _POLARVEC_HPP
#define _POLARVEC_HPP

#include "tbd/log.h"

namespace tomo
{
  /** @brief polar 3D vector defined by th two angles phi, theta and a radial distance
    * @tparam COORD coordinate type
    * @tparam ALLOW_NEG_RADIUS allow negative radius
    * @tparam DEFAULT_COORD_ONE if true, conversion to 4D-vector will 
    *         append a 1.0 and if false a 0.0 as forth coordinate
    */
  template<class COORD, bool ALLOW_NEG_RADIUS=false, bool DEFAULT_COORD_ONE=false> struct PolarVec 
  {
    /// coordinate type
    typedef COORD Coord;
    /// substitute cartesian vector type
    typedef tomo::Vec<3,Coord> Vec;
    /// generate cartesian vector type with for dimensions
    typedef tomo::Vec<4,Coord> Vec4;
    /// number of dimensions
    static const int dimensions_ = 3;
    /// default constructor
    PolarVec() {}
    /** constructor which takes angles and radius
      * @param _phi Phi angle (90°-longitude)
      * @param _theta theta angle (latitude)
      * @param _radius length of the vector
      */
    PolarVec( Coord _longitude, Coord _latitude, Coord _radius ) :
      longitude_(_longitude),
      latitude_(_latitude),
      radius_(_radius)
    {
    }
    /** @brief return cartesian vector from this polar vector
      */
    operator Vec() const
    {
      // get radian latitude
      Coord phi = deg2rad(longitude());
      // get radian longitude
      Coord theta = deg2rad(latitude());
      // calculate and return cartesian vector
      return radius()*Vec(
               sin(theta) * cos(phi),
               sin(theta) * sin(phi),
               cos(theta)
             );
    }

    /** @brief return cartesian 4D vector from polar vector
      * @details depending on DEFAULT_COORD_ONE the additional coordinate 
      * will be 1.0 or 0.0
      */
    operator Vec4() const
    {
      return Vec4(*this,DEFAULT_COORD_ONE?1.0:0.0);
    }
    /** @brief add another polar vector
      * @param _vec other vector
      * @return this instance
      */
    const PolarVec& operator+=( const PolarVec& _vec )
    {
      // add longitude
      longitude_ += _vec.longitude_;
      // add latitude 
      latitude_ = std::min<Coord>(179.99,std::max<Coord>(0.01,latitude_ + _vec.latitude_));
      // add radius
      radius_ += _vec.radius_;//std::max<Coord>(0,radius_ + _vec.radius_) ;
      return *this;
    }
    operator std::string() const
    {
      return operator Vec();
    }

    TBD_PROPERTY(Coord,longitude);
    TBD_PROPERTY(Coord,latitude);
    TBD_PROPERTY(Coord,radius);
  };
  namespace
  {
    template<class COORD> inline fmt& operator%(fmt _fmt, const tomo::PolarVec<COORD>& _pvec)
    {
      return _fmt % (std::string)_pvec;
    }
  }
}
#endif /* _POLARVEC_HPP */
