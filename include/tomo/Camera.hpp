#pragma once
#include "tomo/SphericalTracker.hpp"

namespace tomo
{
  template<int DIMENSIONS, class COORD, class ANGLE=COORD> struct SphericalCamera :
      SphericalTracker<DIMENSIONS,COORD,ANGLE>
  {
    typedef SphericalTracker<DIMENSIONS,COORD> Base;
    /// coordinate type
    typedef COORD Coord;
    /// angle type
    typedef ANGLE Angle;
    /// 3D point
    typedef tomo::Point<DIMENSIONS,COORD> Point;
    /// dimensions
    static const int dimensions_ = DIMENSIONS;

    /** @brief setup camera parameters
     * @param _longitude initial longitude
     * @param _latitude initial latitude_
     * @param _near minimum visible distance to object
     * @param _far maximum visible distance from object
     * @param _distance initial distance from object
     * @param _up camera orientation
     */
    void setup( Angle _longitude, Angle _latitude, Coord _near, Coord _far, Coord _distance, const Point& _up )
    {
      Base::setup(_longitude, _latitude, _near, _far, _distance);
      up_ = _up;
      Base::update();
    }

    /// camera orientation
    TBD_PROPERTY(Point,up);
  };
}
