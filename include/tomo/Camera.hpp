#pragma once
#include "tomo/Ray.hpp"
#include <math.h>
#include <algorithm>

namespace tomo
{
  /** @brief camera base class template
   * @tparam COORD coordinate skalar type
   */
  template<class COORD> struct Camera
  {
  };

  /** @brief spherical camera
   * @tparam COORD coordinate skalar type
   */
  template<class COORD> struct SphericCamera : Camera<COORD>
  {
    /// coordinate type
    typedef COORD Coord;
    /// angle type
    typedef COORD Angle;
    /// 3D point
    typedef tomo::Point<3,COORD> Point;

    /** @brief default constructor
     */
    SphericCamera() : center_(0,0,0), latitude_(0), longitude_(0), distance_(0), up_(0,1,0) { }

    /** @brief setup camera parameters
     * @param _longitude initial longitude
     * @param _latitude initial latitude_
     * @param _near minimum visible distance to object
     * @param _far maximum visible distance from object
     * @param _distance initial distance from object
     */
    void setup( Angle _longitude, Angle _latitude, Coord _near, Coord _far, Coord _distance )
    {
      longitude_ = _longitude;
      latitude_ = _latitude;
      near_ = _near;
      far_ = _far;
      distance_ = _distance;
    }

    /** @brief relatively move the camera on the sphere
     * @param _relativeLongitude relative longitude move in degrees
     * @param _relativeLatitude relative latitude move in degrees
     */
    void move( Angle _relativeLongitude, Angle _relativeLatitude )
    {
      longitude_ += _relativeLongitude;
      latitude_ = std::max(std::min(latitude_ + _relativeLatitude,(Coord)90),(Coord)-90);
    }

    /** @brief change distance between camera and center
     * @param _relativeDistance relative distance move
     * @param _border let gap between near and far distance when zooming
     */
    void zoom( Coord _relativeDistance, Coord _border=0 )
    {
      distance_ = std::min(std::max(distance_ + _relativeDistance, near_ + _border),far_ - _border);
    }

    /** @brief update eye property based on spherical coordinates
     */
    void update()
    {
      /// @todo optimize: move the division to getter/setter of meridian and latiude?!
      eye_.x( center_.x() + distance_ * cos( latitude_ / 180.0 * M_PI ) * cos( longitude_ / 180.0 * M_PI ) );
      eye_.y( center_.y() + distance_ * sin( latitude_ / 180.0 * M_PI ) );
      eye_.z( center_.z() + distance_ * cos( latitude_ / 180.0 * M_PI ) * sin( longitude_ / 180.0 * M_PI ) );
    }

    /// point of interest
    TBD_PROPERTY(Point,center);
    /// latitude of the camera postition on a sphere arround center
    TBD_PROPERTY(Angle,latitude);
    /// longitude/meridian of the camera position on a sphere arround center
    TBD_PROPERTY(Angle,longitude);
    /// distance of the camera from center
    TBD_PROPERTY(Coord,distance);
    /// camera orientation
    TBD_PROPERTY(Point,up);
    /// near z-clipping / maximum zoom
    TBD_PROPERTY(Coord,near);
    /// far z-clipping / minimum zoom
    TBD_PROPERTY(Coord,far);
    /// camera position
    TBD_PROPERTY(Point,eye);
  };
}
