#ifndef _POINT_HPP
#define _POINT_HPP

#include "tomo/Coord.hpp"
#include <boost/static_assert.hpp>
#include <tbd/log.h>

namespace tomo
{
  /** @brief Template class to represent a point
   * @tparam DIMENSIONS Number of dimensions
   * @tparam Scalar_TYPE Coordinate type
   */
  template<int DIMENSIONS, typename SCALAR = DEFAULT_TYPE>
  struct Point : public Coords<DIMENSIONS,SCALAR>
  {
    typedef SCALAR Scalar;
    typedef Scalar value_type;
    typedef Vec<DIMENSIONS,Scalar> vector_type;
    typedef Coords<DIMENSIONS,Scalar> Coords;

    Point() : Coords() {}
    Point( Coords& p ) : Coords( p ) {}
    Point( const Coords& p ) : Coords( p ) {}
    Point( value_type _x, value_type _y ) : Coords(_x,_y) { }
    Point( value_type _x, value_type _y, value_type _z ) : Coords(_x,_y,_z) { }
    Point( value_type _x, value_type _y, value_type _z, value_type _w ) : Coords(_x,_y,_z,_w) { }

    friend vector_type operator-( const Point& a, const Point& b)
    {
      vector_type v;
      TOMO_FOREACH_DIM v[i] = a[i]-b[i];
      return v;
    }
    friend Point operator+( const Point& a, const Point& b)
    {
      Point p;
      TOMO_FOREACH_DIM p[i] = a[i] + b[i];
      return p;
    }
    friend Point operator+( const Point& a, const vector_type& b)
    {
      Point p;
      TOMO_FOREACH_DIM p[i] = a[i] + b[i];
      return p;
    }

    operator const Scalar*() const
    {
      return this->a_;
    }

    Point operator-() const
    {
      return Point() - *this;
    }
    /* @brief Transform point into a vector */
    vector_type vec() const
    {
      vector_type v;
      TOMO_FOREACH_DIM v[i] = this->a_[i];
      return v;
    }
  };

  typedef Point<2,int> Point2i;
  typedef Point<2,double> Point2d;
  typedef Point<2,float> Point2f;
  typedef Point<2,unsigned short> Point2us;
  typedef Point<3,float> Point3f;
}

#endif /* _POINT_HPP */
