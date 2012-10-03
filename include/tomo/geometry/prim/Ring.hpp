#pragma once

#include "tomo/geometry/aux/Compound.hpp"
#include "LineSegment.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      struct Ring : public aux::Compound<LineSegment,2,float>
      {
        TOMO_COMPOUND_PRIMITIVE_NAME(lineSegments);
        
        typedef enum { INNER, OUTER } Location;
        
        Ring(Location _location = OUTER) : location_(_location) {}

        void add(point_type _point);

        bool intersect(ray_type& _ray, scalar_type& _tNear, scalar_type& _tFar, vector_type* _normal = NULL) const
        {
          return false;
        }

        SplitPlaneIntersect intersect(base::Axis _axis, scalar_type _splitPos,
                                      const bounds_type& _boundsLeft, const bounds_type& _boundsRight) const
        {
          return SplitPlaneIntersect();
        }

        friend void resize(Ring& _in, scalar_type _distance, std::vector<Ring>& _out);
        friend void difference(Ring& _in, std::vector<Ring>& _out);
        friend void unification(Ring& _in, std::vector<Ring>& _out);
        friend bool intersect(Ring& _ring1, Ring& _ring2);
      
        TBD_PROPERTY(Location,location);
      };

    }
  }
}

