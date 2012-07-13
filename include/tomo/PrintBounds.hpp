#pragma once
#include "Bounds.hpp"

namespace tomo
{
  struct PrintBounds : Bounds3f
  {
    typedef Vec3f Vec;
    typedef Point3f Point;
    PrintBounds() {}
    PrintBounds( const Vec& _size ) : 
      Bounds( Point( -_size.x()/2, -_size.y()/2, 0 ),
          Point( _size.x()/2, _size.y()/2, _size.z() ) )
    {
    }
  };
}

