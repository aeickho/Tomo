#pragma once
#include "tomo/IMouse.hpp"
#include <algorithm>

namespace tomo
{
  /** @brief spherical camera
   * @tparam DIMENSIONS number of dimensions of points
   * @tparam COORD coordinate skalar type
   */
  template<int DIMENSIONS, class COORD> struct Tracker :
      IMouse
  {
    /// coordinate type
    typedef COORD Coord;
    /// 3D point
    typedef tomo::Point<DIMENSIONS,COORD> Point;
    /// dimensions
    static const int dimensions_ = DIMENSIONS;

    /** @brief setup camera parameters
     * @param _near minimum visible distance to object
     * @param _far maximum visible distance from object
     * @param _distance initial distance from object
     */
    void setup( const Point& _pos, Coord _near, Coord _far, Coord _distance )
    {
      pos_ = _pos;
      near_ = _near;
      far_ = _far;
      distance_ = _distance;
    }

    virtual void drag( int _x, int _y )
    {
      pos_.x( pos_.x() + (Coord)_x );
      pos_.y( pos_.y() + (Coord)_y );
    }

    virtual void wheel( int _d )
    {
      distance_ = std::min(std::max(distance_ + (Coord)_d, near_),far_);
    }

    /// point of interest
    TBD_PROPERTY(Point,center);
    /// distance of the camera from center
    TBD_PROPERTY(Coord,distance);
    /// near z-clipping / maximum zoom
    TBD_PROPERTY(Coord,near);
    /// far z-clipping / minimum zoom
    TBD_PROPERTY(Coord,far);
    /// positon in Cartesian coordiantes
    TBD_PROPERTY(Point,pos);
  };
}
