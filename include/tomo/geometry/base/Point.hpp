#ifndef _POINT_HPP
#define _POINT_HPP

#include "Coord.hpp"
#include <boost/geometry/geometries/point_xy.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace base
    { 
      typedef boost::geometry::model::d2::point_xy<DEFAULT_TYPE> BoostPoint2;

      /** @brief Template class to represent a point
       * @tparam DIMENSIONS Number of dimensions
       * @tparam Scalar_TYPE Coordinate type
       */
      template<int DIMENSIONS, typename SCALAR = DEFAULT_TYPE>
      struct Point : public Coords<DIMENSIONS,SCALAR>
      {
        typedef SCALAR Scalar;
        typedef Scalar value_type;
        typedef Vec<DIMENSIONS,SCALAR> vector_type;
        typedef Coords<DIMENSIONS,SCALAR> coord_type;

        Point() : coord_type() {}
        Point( coord_type& p ) : coord_type( p ) {}
        Point( const coord_type& p ) : coord_type( p ) {}
        Point( value_type _x, value_type _y ) : coord_type(_x,_y) { }
        Point( value_type _x, value_type _y, value_type _z ) : coord_type(_x,_y,_z) { }
        Point( value_type _x, value_type _y, value_type _z, value_type _w ) : coord_type(_x,_y,_z,_w) { }
        Point ( BoostPoint2 _boostPoint ) : 
          coord_type(boost::geometry::get<0>(_boostPoint),
                     boost::geometry::get<1>(_boostPoint))
        {}


        friend vector_type operator-( const Point& a, const Point& b)
        {
          vector_type v;
          TOMO_FOREACH_DIM(i) v[i] = a[i]-b[i];
          return v;
        }
        friend Point operator+( const Point& a, const Point& b)
        {
          Point p;
          TOMO_FOREACH_DIM(i) p[i] = a[i] + b[i];
          return p;
        }
        friend Point operator+( const Point& a, const vector_type& b)
        {
          Point p;
          TOMO_FOREACH_DIM(i) p[i] = a[i] + b[i];
          return p;
        }

        BoostPoint2 as() const 
        {
          BOOST_STATIC_ASSERT(DIMENSIONS == 2);
          return BoostPoint2(this->x(),this->y());
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
          TOMO_FOREACH_DIM(i) v[i] = this->a_[i];
          return v;
        }

        Point<DIMENSIONS-1,SCALAR> project(Axis _axis)
        {
          Point<DIMENSIONS-1,SCALAR> _projPoint;
          int _dim = 0;
          TOMO_FOREACH_DIM(i)
          {
            if (i == _axis) continue;
            _projPoint[_dim] = this->a_[i];
            _dim++;
          }
          return _projPoint;
        }
      };

      typedef Point<2,int> Point2i;
      typedef Point<2,double> Point2d;
      typedef Point<2,float> Point2f;
      typedef Point<2,unsigned short> Point2us;
      typedef Point<3,float> Point3f;
    }
  }
}
#endif /* _POINT_HPP */
