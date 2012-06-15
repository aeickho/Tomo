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
 /*   SplitPlane plane_;
    std::vector<Isle> isles_;

    Ray generateRay(float _pos)
    {

    }

    vector<Ray> generateRays(unsigned _numberOfRays)
    {
      std::vector<Ray> _rays; _rays.reserve(_numberOfRays);
      Vec3f _dir(plane_.u_.cross(plane_.n_)), 
            _org(plane_.v_.cross(plane_.n_));
      diff = (_org - plane.center_) * (0.5 / _numberOfRays);

      for (unsigned i = 0; i < _numberOfRays; i++)
       {
        _rays.push_back(Ray(_org,_dir,0.0,1.0));
        _org += diff; 
      }
    }
*/

  };

  typedef vector<Slice> SliceList;
  typedef vector<Slice*> Slices;
}
