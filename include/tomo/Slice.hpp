#pragma once

#include <vector>
#include "tomo/Vector.hpp"

namespace tomo
{
  using std::vector;

  typedef vector<Point2f> Polyline;
  typedef vector<Polyline> Polylines;
  typedef vector<Polyline*> PolylineList;

  class Track : public Polyline
  {
  };

  struct Slice 
  {
    Polylines border;
    Polylines innerSupport, outerSupport;	  

    float z;
  };

  typedef vector<Slice> SliceList;
  typedef vector<Slice*> Slices;
}
