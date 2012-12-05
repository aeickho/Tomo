#ifndef _POINT_HPP
#define _POINT_HPP

#include "Coord.hpp"
#include "Matrix.hpp"

#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/register/point.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace base
    {
      /** @brief Template class to represent a point
       * @tparam dimensions_ Number of dimensions
       * @tparam Scalar_TYPE Coordinate type
       */
      template<class MODEL>
      struct Point : public Coords<MODEL>
      {
        TOMO_MODEL_TYPES(MODEL);

        typedef Coords<MODEL> coords_type;
        typedef Vec<MODEL> vector_type;
        typedef Matrix<Model<dimensions_+1,scalar_type>> matrix_type;

        Point() : coords_type() {}
        Point( coords_type& p ) : coords_type( p ) {}
        Point( const coords_type& p ) : coords_type( p ) {}
        Point( scalar_type _x, scalar_type _y ) : coords_type(_x,_y) { }
        Point( scalar_type _x, scalar_type _y, scalar_type _z ) : coords_type(_x,_y,_z) { }
        Point( scalar_type _x, scalar_type _y, scalar_type _z, scalar_type _w ) : coords_type(_x,_y,_z,_w) { }

        friend Point operator*( const matrix_type& _m, const Point& _p)
        {
          Point _point;
          TOMO_FOREACH_DIM(i)
          {
            for (int j = 0; j < dimensions_+1; j++)
            {
              _point[i] += (j == dimensions_) ? _m(i,j) : _m(i,j) * _p[j];
            }
          }
          return _point;
        }

        friend Point operator*( const Point& _p, const matrix_type& _m)
        {
          return _m * _p;
        }

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

        operator const scalar_type*() const
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

        Point<Model<dimensions_-1,scalar_type>> project(Axis _axis) const
        {
          Point<Model<dimensions_-1,scalar_type>> _projPoint;
          int _dim = 0;
          TOMO_FOREACH_DIM(i)
          {
            if (i == _axis) continue;
            _projPoint[_dim] = Point::a_[i];
            _dim++;
          }
          return _projPoint;
        }

      };

      typedef Point<Model2i> Point2i;
      typedef Point<Model2d> Point2d;
      typedef Point<Model2f> Point2f;
      typedef Point<Model2us> Point2us;
      typedef Point<Model3f> Point3f;
    }
  }
}

BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(tomo::geometry::base::Point2f,float,cs::cartesian,x,y,x,y)



#endif /* _POINT_HPP */
