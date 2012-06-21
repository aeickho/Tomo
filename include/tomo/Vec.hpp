#ifndef _VEC_HPP
#define _VEC_HPP

#include "tomo/Coord.hpp"
#include <boost/static_assert.hpp>
#include <algorithm>
#include <boost/mpl/int.hpp>

namespace tomo
{
  /** @brief Template class to represent a vector
   * @tparam DIMENSIONS Number of dimensions
   * @tparam COORD_TYPE Coordinate type
   */
  template<int DIMENSIONS, typename COORD = DEFAULT_TYPE>
  struct Vec : public Coords<DIMENSIONS,COORD>
  {
    /// Coordinate value type
    typedef COORD Coord;
    /// Type of base class
    typedef Coords<DIMENSIONS,COORD> _Coords;

    Vec() : _Coords() {}
    Vec( Vec& v ) : _Coords( v ) {}
    Vec( const Vec& v ) : _Coords( v ) {}
    Vec( Coord _x, Coord _y ) : _Coords(_x,_y) { }
    Vec( Coord _x, Coord _y, Coord _z) : _Coords(_x,_y,_z) { }
    Vec( Coord _x, Coord _y, Coord _z, Coord _w ) : _Coords(_x,_y,_z,_w) { }

    /// @todo [fightling] is length correct when w() != 1.0 ??
    Coord sqrLength() const
    {
      return this->dot(*this);
    }
    Coord	length() const
    {
      return sqrt( sqrLength() );
    }
    void normalize()
    {
      Coord l = length();
      if (l>0.0f) TOMO_FOREACH_DIM this->a_[i] /= l;
    }
    Vec normalized() const
    {
      Vec v(*this);
      v.normalize();
      return v;
    }

    /** @brief Calculates cross product by this and another Vec
     * @param v   Second vector
     * @returns Vector
     * @details Number of dimension must be 3!
     **/
    const Vec cross(const Vec& v) const
    {
      BOOST_STATIC_ASSERT(DIMENSIONS == 3);
      return Vec(this->y()*v.z() - this->z()*v.y(),
                 this->z()*v.x() - this->x()*v.z(),
                 this->x()*v.y() - this->y()*v.x() );
    }

    /** @brief Calculates dot product by this and another Vec
     * @param v   Second vector
     * @returns Coord value
     **/
    const Coord dot( const Vec& v) const
    {
      Coord sum = 0;
      TOMO_FOREACH_DIM sum += v[i]*this->a_[i];
      return sum;
    }

    /// Vector operations
    Vec operator- () const
    {
      Vec v(*this);
      TOMO_FOREACH_DIM v[i] = -v[i];
      return v;
    }
    void operator *= ( Coord f )
    {
      TOMO_FOREACH_DIM this->a_[i] *= f;
    }

    friend Vec      operator*( const Vec& a, const Coord f )
    {
      Vec v(a);
      TOMO_FOREACH_DIM v[i] *= f;
      return v;
    }
    friend Vec      operator*( const Coord f, const Vec& a )
    {
      return a*f;
    }
    friend Vec      operator*( const Vec& a, const Vec& b)
    {
      Vec v;
      TOMO_FOREACH_DIM v[i] = a[i]*b[i];
      return v;
    }
    friend Vec      operator-( const Vec& a, const Vec& b)
    {
      Vec v;
      TOMO_FOREACH_DIM v[i] = a[i]-b[i];
      return v;
    }
    friend Vec      operator+( const Vec& a, const Vec& b)
    {
      Vec v;
      TOMO_FOREACH_DIM v[i] = a[i]+b[i];
      return v;
    }
    /*
        friend Vec      operator*( const Vec& a, const Matrix<DIMENSIONS+1,Coord>& M )
        {
          _Vec v;
          v.x = a.x * M.get(0,0) + a.y*M.get(1,0) + a.z*M.get(2,0) + M.get(3,0);
          v.y = a.x * M.get(0,1) + a.y*M.get(1,1) + a.z*M.get(2,1) + M.get(3,1);
          v.z = a.x * M.get(0,2) + a.y*M.get(1,2) + a.z*M.get(2,2) + M.get(3,2);
          return v;
        }
    */
  };

  typedef Vec<2,int> Vec2i;
  typedef Vec<2,double> Vec2d;
  typedef Vec<2,float> Vec2f;
  typedef Vec<3,float> Vec3f;
}

#endif /* _VEC_HPP */
