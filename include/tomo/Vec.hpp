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
  template<int DIMENSIONS, typename SCALAR = DEFAULT_TYPE>  
  struct Vec : public Coords<DIMENSIONS,SCALAR>
  {
    /// Coordinate value type
    typedef SCALAR Scalar;
    typedef Scalar value_type;

    /// Type of base class
    typedef Coords<DIMENSIONS,Scalar> _Coords;
    typedef Vec<DIMENSIONS,Scalar> vector_type;

    Vec() : _Coords() {}
    Vec( Vec& v ) : _Coords( v ) {}
    Vec( const Vec& v ) : _Coords( v ) {}
    Vec( value_type _x, value_type _y ) : _Coords(_x,_y) { }
    Vec( value_type _x, value_type _y, value_type _z) : _Coords(_x,_y,_z) { }
    Vec( value_type _x, value_type _y, value_type _z, value_type _w ) : _Coords(_x,_y,_z,_w) { }

    Scalar  sqrLength()   const     { return dot(*this,*this); }
    Scalar	 length() 		const	  	{ return    sqrt( sqrLength() ); }
    vector_type& normalize() { Scalar l = length(); TOMO_FOREACH_DIM this->a_[i] /= l; return *this; }
    vector_type 	     normalized() const		{ return *this * ( 1 / length() ); }

    /** @brief Calculates cross product by this and another Vec
     * @param _left first vector
     * @param _right second vector
     * @returns cross product vector
     * @details Number of dimension must be 3!
     **/
    friend Vec cross(const Vec& _left, const Vec& _right)
    {
      BOOST_STATIC_ASSERT(DIMENSIONS == 3);
      return Vec(_left.y() * _right.z() - _left.z() * _right.y(),
                 _left.z() * _right.x() - _left.x() * _right.z(),
                 _left.x() * _right.y() - _left.y() * _right.x() );
    }      
    
    /** @brief Calculates dot product by this and another Vec
     * @param _left first vector
     * @param _right second vector
     * @returns Coord value
     **/
    friend value_type dot( const Vec& _left, const Vec& _right)
    {
      value_type sum = 0;
      TOMO_FOREACH_DIM 
        sum += _left[i] * _right[i];
      return sum;
    }

    /** @brief Return axis which largest extent
     */
    Axis dominantAxis() const
    {
      Axis _domAxis = X;
      float _max = 0.0;
      TOMO_FOREACH_DIM 
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
    Vec operator- () const { Vec v(*this);  TOMO_FOREACH_DIM v[i] = -v[i];  return v; }
    void operator *= ( value_type f ) 	{  TOMO_FOREACH_DIM this->a_[i] *= f; }

    friend Vec      operator*( const Vec& a, const value_type f ) { Vec v(a); TOMO_FOREACH_DIM v[i] *= f; return v; }
    friend Vec      operator*( const value_type f, const Vec& a ) { return a*f; }
    friend Vec      operator*( const Vec& a, const Vec& b) { Vec v; TOMO_FOREACH_DIM v[i] = a[i]*b[i]; return v; } 
    friend Vec      operator-( const Vec& a, const Vec& b) { Vec v; TOMO_FOREACH_DIM v[i] = a[i]-b[i]; return v; }
    friend Vec      operator+( const Vec& a, const Vec& b) { Vec v; TOMO_FOREACH_DIM v[i] = a[i]+b[i]; return v; }
/*
    friend Vec      operator*( const Vec& a, const Matrix<DIMENSIONS+1,value_type>& M ) 
    { 
      vector_type v;
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
  typedef Vec<2,short> Vec2s;
}

#endif /* _VEC_HPP */
