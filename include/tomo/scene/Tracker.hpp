#pragma once
#include <algorithm>
#include "tomo/geometry/base/Point.hpp"
#include "tomo/geometry/base/PolarVec.hpp"

namespace tomo
{
  namespace scene
  {
    template<
    class COORD,
          bool XNEG=false,
          bool YNEG=false,
          bool ZNEG=false
          > struct Tracker
    {
      typedef COORD Coord;
      /// point type
      typedef geometry::base::Point<3,Coord> Point;
      /// vector type
      typedef geometry::base::Vec<3,Coord> Vec;
      typedef geometry::base::PolarVec<Coord> PolarVec;
      static const bool xNeg_ = XNEG;
      static const bool yNeg_ = YNEG;
      static const bool zNeg_ = ZNEG;

      Tracker()
      {
      }
      Tracker( const Tracker& _tracker )
      {
        center_ = _tracker.center_;
        direction_ = _tracker.direction_;
      }
      Tracker( const Point& _center, const PolarVec& _direction ) :
        center_(_center),
        direction_(_direction)
      {
      }
      void track( int x, int y, int z )
      {
        direction_ += PolarVec(
                        xNeg_?-x:x,
                        yNeg_?-y:y,
                        zNeg_?-z:z);
      }
      /// get tracker position
      Point eye() const
      {
        return center_ + direction_;
      }
      void eye( const Point& _pos )
      {
        direction_ = _pos - center_;
      }
      /// target position
      TBD_PROPERTY_REF(Point,center);
      /// tracker's postion relatively to target
      TBD_PROPERTY_REF(PolarVec,direction);
    };
  }
}
