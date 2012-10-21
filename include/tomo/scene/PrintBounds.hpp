#pragma once
#include "tomo/geometry/base/Bounds.hpp"

namespace tomo
{
  namespace scene
  {
    struct PrintBounds : geometry::base::Bounds3f
    {
      typedef tomo::geometry::base::Vec3f Vec;
      typedef tomo::geometry::base::Point3f Point;
      PrintBounds() {}
      PrintBounds( const Vec& _size ) :
        Bounds( Point( -_size.x()/2, -_size.y()/2, 0 ),
                Point( _size.x()/2, _size.y()/2, _size.z() ) )
      {
      }
    };
  }
}

