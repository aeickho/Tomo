#ifndef _POINT_HPP
#define _POINT_HPP

#include "tomo/Coord.hpp"
#include <boost/static_assert.hpp>
#include <tbd/log.h>

namespace tomo
{
  /** @brief Template class to represent a point 
   * @tparam DIMENSIONS Number of dimensions
   * @tparam COORD_TYPE Coordinate type
   */
  template<int DIMENSIONS, typename COORD = DEFAULT_TYPE>  
  struct Point : public Coords<DIMENSIONS,COORD>
  {
    typedef COORD Coord;
    typedef Vec<DIMENSIONS,COORD> Vec;
    typedef Coords<DIMENSIONS,COORD> Coords;

    Point() : Coords() {}
    Point( Point& p ) : Coords( p ) {}
    Point( const Point& p ) : Coords( p ) {}
    Point( Coord _x, Coord _y ) : Coords(_x,_y) { }
    Point( Coord _x, Coord _y, Coord _z ) : Coords(_x,_y,_z) { }
    Point( Coord _x, Coord _y, Coord _z, Coord _w ) : Coords(_x,_y,_z,_w) { }

    friend Vec operator-( const Point& a, const Point& b) { Vec v; TOMO_FOREACH_DIM v[i] = a[i]-b[i]; return v; }
    friend Point operator+( const Point& a, const Point& b) { Point p; TOMO_FOREACH_DIM p[i] = a[i] + b[i]; return p; }
    friend Point operator+( const Point& a, const Vec& b) { Point p; TOMO_FOREACH_DIM p[i] = a[i] + b[i]; return p; }
    operator const Coord*() const { return this->a_; }
    /* @brief Transform point into a vector */
    Vec vec() { Vec v; TOMO_FOREACH_DIM v[i] = this->a_[i]; return v; }
    operator std::string() const
    {
      return Coords::operator std::string();
    }
  };
  namespace 
  {
    template<int DIMENSIONS, class COORD> inline fmt operator%(fmt _fmt, const tomo::Point<DIMENSIONS,COORD>& _point)
    {
      return _fmt % (std::string)_point;
    }
  }
  typedef Point<2,int> Point2i;
  typedef Point<2,double> Point2d;

  typedef Point<2,float> Point2f;
  typedef Point<3,float> Point3f;
}

#endif /* _POINT_HPP */
