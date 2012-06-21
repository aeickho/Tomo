#ifndef _COORD_HPP
#define _COORD_HPP

#include "tomo/misc.hpp"
#include <boost/static_assert.hpp>

namespace tomo
{
  class Matrix4f
  {
    public:
      Matrix4f() 
      { 
        for (int i = 0; i < 4; i++) 
          for (int j = 0; j < 4; j++)
            set(i,j,(i == j) ? 1.0f : 0.0f);	
      };

      void  set(int _x, int _y, float v) { c[_x*4+_y] = v; } 
      float get(int _x, int _y) { return c[_x*4+_y]; }

      void  print();

    private:
      float c[4*4];
  };

  /// DEFAULT_TYPE of coordinates is float
  typedef float DEFAULT_TYPE;

/// Compiler macro to traverse through each dimension
#define TOMO_FOREACH_DIM for (int i = 0; i < DIMENSIONS; i++) 

  /** @brief Base class of Point and Vec which basically hold a number of coordinates
   * @tparam DIMENSIONS Number of dimensions
   * @tparam COORD_TYPE Coordinate type
   */
  template<int DIMENSIONS, typename COORD_TYPE = DEFAULT_TYPE>  
  struct Coords
  {
    /// Coordinate value type
    typedef COORD_TYPE CoordType;
    
    /// Base constructor, all values are initially set to zero
    Coords() { TOMO_FOREACH_DIM a_[i] = 0; }
    Coords( Coords& _coords ) { TOMO_FOREACH_DIM a_[i] = _coords[i]; }
    Coords( const Coords& _coords ) { TOMO_FOREACH_DIM a_[i] = _coords[i]; }
    Coords( CoordType _x, CoordType _y) { this->operator()(_x,_y); }
    Coords( CoordType _x, CoordType _y, CoordType _z) { this->operator()(_x,_y,_z); }
    Coords( CoordType _x, CoordType _y, CoordType _z, CoordType _w) { this->operator()(_x,_y,_z,_w); }
   
    inline void operator()(const Coords _coords) { TOMO_FOREACH_DIM a_[i] = _coords[i]; } 
    inline void operator()(CoordType _x, CoordType _y) { x(_x); y(_y); }
    inline void operator()(CoordType _x, CoordType _y, CoordType _z) { x(_x); y(_y); z(_z); }
    inline void operator()(CoordType _x, CoordType _y, CoordType _z, CoordType _w) { x(_x); y(_y); z(_z); w(_w); }
    
    /* @brief Return pointer */
    CoordType* p() { return a_; }
    /* @brief Return const pointer */
    const CoordType* p() const { return a_; }

    /// Methods to return coordinate values
    inline CoordType x() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 1); return a_[0]; }
    inline CoordType y() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 2); return a_[1]; }
    inline CoordType z() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 3); return a_[2]; }
    inline CoordType w() const { BOOST_STATIC_ASSERT(DIMENSIONS >= 4); return a_[3]; }
    
    /// Methods to set coordinate values 
    inline void x(const CoordType _x) { BOOST_STATIC_ASSERT(DIMENSIONS >= 1); a_[0] = _x; }
    inline void y(const CoordType _y) { BOOST_STATIC_ASSERT(DIMENSIONS >= 2); a_[1] = _y; }
    inline void z(const CoordType _z) { BOOST_STATIC_ASSERT(DIMENSIONS >= 3); a_[2] = _z; }
    inline void w(const CoordType _w) { BOOST_STATIC_ASSERT(DIMENSIONS >= 4); a_[3] = _w; }

    /// Methods to access coordinate value in a certain dimension
    CoordType& operator[] (int i) { return a_[i]; }
    const CoordType& operator[] (int i) const { return a_[i]; }

    void operator += ( const Coords _c ) { TOMO_FOREACH_DIM a_[i] += _c[i]; }
    void operator -= ( const Coords _c ) { TOMO_FOREACH_DIM a_[i] -= _c[i]; } 

    protected:
    /// Array to store coordinate values
    CoordType a_[DIMENSIONS]; 
  };

  typedef enum { X,Y,Z,W } Axis;

  //typedef Matrix<float> Matrix4f;

  #define COORDS(C) C.x(),C.y(),C.z() 
}

#endif /* _COORD_HPP */
