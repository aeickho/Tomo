#ifndef _VEC_HPP
#define _VEC_HPP

#include "Coord.hpp"
#include <tbd/log.h>
#include <boost/static_assert.hpp>
#include <boost/mpl/int.hpp>
#include <algorithm>

namespace tomo
{
  namespace geometry
  {
    namespace base
    {
      /** @brief Template class to represent a vector
       * @tparam DIMENSIONS Number of dimensions
       * @tparam COORD_TYPE Coordinate type
       */
      template<class MODEL>
      struct Vec : Coords<MODEL>
      {
        TOMO_MODEL_TYPES(MODEL)
        typedef Coords<MODEL> coords_type;
        typedef Vec<MODEL> vector_type;

        Vec() : coords_type() {}
        Vec( Vec& v ) : coords_type( v ) {}
        Vec( const Vec& v ) : coords_type( v ) {}
        Vec( scalar_type _x, scalar_type _y ) : coords_type(_x,_y) { }
        Vec( scalar_type _x, scalar_type _y, scalar_type _z) : coords_type(_x,_y,_z) { }
        Vec( scalar_type _x, scalar_type _y, scalar_type _z, scalar_type _w ) : coords_type(_x,_y,_z,_w) { }

        scalar_type sqrLength()   const
        {
          return dot(*this,*this);
        }
        scalar_type length() 		const
        {
          return    sqrt( sqrLength() );
        }
        vector_type& normalize()
        {
          scalar_type l = length();
          TOMO_FOREACH_DIM(i) this->a_[i] /= l;
          return *this;
        }
        vector_type 	     normalized() const
        {
          return *this * ( 1 / length() );
        }

        /** @brief Calculates cross product by this and another Vec
         * @param _left first vector
         * @param _right second vector
         * @returns cross product vector
         * @details Number of dimension must be 3!
         **/
        friend Vec cross(const Vec& _left, const Vec& _right)
        {
          BOOST_STATIC_ASSERT(dimensions_ == 3);
          return Vec(_left.y() * _right.z() - _left.z() * _right.y(),
                     _left.z() * _right.x() - _left.x() * _right.z(),
                     _left.x() * _right.y() - _left.y() * _right.x() );
        }

        /** @brief Calculates dot product by this and another Vec
         * @param _left first vector
         * @param _right second vector
         * @returns Coord value
         **/
        friend scalar_type dot( const Vec& _left, const Vec& _right)
        {
          scalar_type sum = 0;
          TOMO_FOREACH_DIM(i)
          sum += _left[i] * _right[i];
          return sum;
        }

        /** @brief Return axis which largest extent
         */
        Axis dominantAxis() const
        {
          Axis _domAxis = X;
          float _max = 0.0;
          TOMO_FOREACH_DIM(i)
          {
            float _abs = abs(this->a_[i]);
            if (_abs > _max)
            {
              _max = _abs;
              _domAxis = Axis(i);
            }
          }
          return _domAxis;
        }

        /// Vector operations
        Vec operator- () const
        {
          Vec v(*this);
          TOMO_FOREACH_DIM(i) v[i] = -v[i];
          return v;
        }
        void operator *= ( scalar_type f )
        {
          TOMO_FOREACH_DIM(i) this->a_[i] *= f;
        }

        friend Vec operator*( const Vec& a, const scalar_type f )
        {
          Vec v(a);
          TOMO_FOREACH_DIM(i) v[i] *= f;
          return v;
        }
        friend Vec operator*( const scalar_type f, const Vec& a )
        {
          return a*f;
        }
        friend Vec operator*( const Vec& a, const Vec& b)
        {
          Vec v;
          TOMO_FOREACH_DIM(i) v[i] = a[i]*b[i];
          return v;
        }
        friend Vec operator-( const Vec& a, const Vec& b)
        {
          Vec v;
          TOMO_FOREACH_DIM(i) v[i] = a[i]-b[i];
          return v;
        }
        friend Vec operator+( const Vec& a, const Vec& b)
        {
          Vec v;
          TOMO_FOREACH_DIM(i) v[i] = a[i]+b[i];
          return v;
        }
        /*
            friend Vec      operator*( const Vec& a, const Matrix<DIMENSIONS+1,scalar_type>& M )
            {
              vector_type v;
              v.x = a.x * M.get(0,0) + a.y*M.get(1,0) + a.z*M.get(2,0) + M.get(3,0);
              v.y = a.x * M.get(0,1) + a.y*M.get(1,1) + a.z*M.get(2,1) + M.get(3,1);
              v.z = a.x * M.get(0,2) + a.y*M.get(1,2) + a.z*M.get(2,2) + M.get(3,2);
              return v;
            }
        */
      };

      typedef Vec<Model2i> Vec2i;
      typedef Vec<Model2d> Vec2d;
      typedef Vec<Model2f> Vec2f;
      typedef Vec<Model2us> Vec2us;
      typedef Vec<Model2s> Vec2s;
      typedef Vec<Model3f> Vec3f;
    }
  }
}
#endif /* _VEC_HPP */
