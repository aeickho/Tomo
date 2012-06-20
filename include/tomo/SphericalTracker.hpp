#pragma once
#include "tomo/Tracker.hpp"
#include <math.h>

namespace tomo
{
  /** @brief spherical camera
   * @tparam COORD coordinate skalar type
   */
  template<int DIMENSIONS, class COORD, class ANGLE=COORD> struct SphericalTracker :
      Tracker<DIMENSIONS,COORD>
  {
    typedef Tracker<DIMENSIONS,COORD> Base;
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
     */
    void setup(Angle _longitude, Angle _latitude, Coord _near, Coord _far, Coord _distance )
    {
      Base::setup(Point(), _near, _far, _distance);
      longitude_ = _longitude;
      latitude_ = _latitude;
      update();
    }

    /** @brief relatively move the camera on the sphere
     * @param _x relative longitude move in degrees
     * @param _y relative latitude move in degrees
     */
    virtual void drag( int _x, int _y )
    {
      longitude_ += (Coord)_x;
      latitude_ = std::max(std::min(latitude_ + (Coord)_y,(Coord)90),(Coord)-90);
      update();
    }

    virtual void wheel( int _d )
    {
      Base::wheel(_d);
      update();
    }

    /** @brief update eye property based on spherical coordinates
     */
    void update()
    {
      BOOST_STATIC_ASSERT(dimensions_ >= 3);
      Base::pos(
        Point(
          Base::center().x() + Base::distance() * cos( latitude_ / 180.0 * M_PI ) * cos( longitude_ / 180.0 * M_PI ),
          Base::center().y() + Base::distance() * sin( latitude_ / 180.0 * M_PI ),
          Base::center().z() + Base::distance() * cos( latitude_ / 180.0 * M_PI ) * sin( longitude_ / 180.0 * M_PI ) 
          ) 
        );
    }

    /// latitude of the camera postition on a sphere arround center
    TBD_PROPERTY(Angle,latitude);
    /// longitude/meridian of the camera position on a sphere arround center
    TBD_PROPERTY(Angle,longitude);
  };
}
