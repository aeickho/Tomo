#ifndef _VEC_HPP
#define _VEC_HPP

#include <tomo/Coord.hpp>
#include <tbd/log.h>
#include <boost/static_assert.hpp>
#include <boost/mpl/int.hpp>
#include <algorithm>

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
    typedef Coords<DIMENSIONS,COORD> Coords;

    Vec() : Coords() {}
    Vec( Vec& v ) : Coords( v ) {}
    Vec( const Vec& v ) : Coords( v ) {}
    Vec( const Coords& c ) : Coords( c ) {}
    Vec( Coord _x, Coord _y ) : Coords(_x,_y) { }
    Vec( Coord _x, Coord _y, Coord _z) : Coords(_x,_y,_z) { }
    Vec( Coord _x, Coord _y, Coord _z, Coord _w ) : Coords(_x,_y,_z,_w) { }
    Vec( const tomo::Coords<DIMENSIONS-1,COORD>& _coords, Coord _c=1.0 ) : Coords(_coords, _c) {}

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
     * @param _left first vector
     * @param _right second vector
     * @returns cross product vector
     * @details Number of dimension must be 3!
     **/
    friend Vec cross_(const Vec& _left, const Vec& _right)
    {
      BOOST_STATIC_ASSERT(DIMENSIONS == 3);
      return Vec(_left.y() * _right.z() - _left.z() * _right.y(),
                 _left.z() * _right.x() - _left.x() * _right.z(),
                 _left.x() * _right.y() - _left.y() * _right.x() );
    }
    /// @deprecated use friend function instead 
    Vec cross(const Vec& v) const
    {
      return cross_( *this, v );
    }

    /** @brief Calculates dot product by this and another Vec
     * @param _left first vector
     * @param _right second vector
     * @returns Coord value
     **/
    friend Coord dot_( const Vec& _left, const Vec& _right)
    {
      Coord sum = 0;
      TOMO_FOREACH_DIM 
        sum += _left[i] * _right[i];
      return sum;
    }
    /// @deprecated use friend function instead 
    Coord dot( const Vec& v) const
    {
      return dot_( *this, v );
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
