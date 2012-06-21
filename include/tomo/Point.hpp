#ifndef _POINT_HPP
#define _POINT_HPP

#include "tomo/Coord.hpp"
#include <boost/static_assert.hpp>

namespace tomo
{
  /** @brief Template class to represent a point 
   * @tparam DIMENSIONS Number of dimensions
   * @tparam COORD_TYPE Coordinate type
   */
  template<int DIMENSIONS, typename COORD_TYPE = DEFAULT_TYPE>  
  struct Point : public Coords<DIMENSIONS,COORD_TYPE>
  {
    typedef COORD_TYPE CoordType;
    typedef Vec<DIMENSIONS,COORD_TYPE> _Vec;
    typedef Coords<DIMENSIONS,COORD_TYPE> _Coords;

    Point() : _Coords() {}
    Point( Point& p ) : _Coords( p ) {}
    Point( const Point& p ) : _Coords( p ) {}
    Point( CoordType _x, CoordType _y ) : _Coords(_x,_y) { }
    Point( CoordType _x, CoordType _y, CoordType _z ) : _Coords(_x,_y,_z) { }
    Point( CoordType _x, CoordType _y, CoordType _z, CoordType _w ) : _Coords(_x,_y,_z,_w) { }

    friend _Vec operator-( const Point& a, const Point& b) { _Vec v; TOMO_FOREACH_DIM v[i] = a[i]-b[i]; return v; }
    friend Point operator+( const Point& a, const Point& b) { Point p; TOMO_FOREACH_DIM p[i] = a[i] + b[i]; return p; }
    friend Point operator+( const Point& a, const _Vec& b) { Point p; TOMO_FOREACH_DIM p[i] = a[i] + b[i]; return p; }

    /* @brief Transform point into a vector */
    _Vec vec() { _Vec v; TOMO_FOREACH_DIM v[i] = this->a_[i]; return v; }
  };

  typedef Point<2,int> Point2i;
  typedef Point<2,double> Point2d;

  typedef Point<2,float> Point2f;
  typedef Point<3,float> Point3f;
}

#endif /* _POINT_HPP */
