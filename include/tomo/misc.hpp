#pragma once

#include <tbd/property.h>
#include <limits>
#include <string>
#include <cmath>

namespace tomo
{
  typedef unsigned char u8;
  typedef unsigned int u32;
  typedef unsigned short u16;

  /// use STL strings
  using std::string;

#define RND float(rand())/float(RAND_MAX)
#define U8(a) (a) > 1.0 ? 255 : (a) < 0.0 ? 0 : (u8)((a)*255.0f)

#define INF std::numeric_limits<double>::max()
#define EPSILON 0.000001

#define FOREACH_AXIS for(int axis = 0; axis < 3; axis++)

  template<class COORD> COORD deg2rad( COORD _value )
  {
    return _value * (COORD)(M_PI / 180.0);
  }
  template<class COORD> COORD rad2deg( COORD _value )
  {
    return _value * (COORD)(180.0 / M_PI);
  }
}
