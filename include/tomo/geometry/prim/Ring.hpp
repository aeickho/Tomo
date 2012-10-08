#pragma once

#include <boost/geometry/geometries/ring.hpp>
#include "LineSegment.hpp"
#include "Vertex.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      typedef boost::geometry::model::ring<base::BoostPoint2> BoostRing;

      struct Ring : Primitive2f
      {

        enum Location { INNER, OUTER };

        /// Orientation of ring (CW = clockwise, CCW = counter-clockwise)
        enum Orientation { CW, CCW };

        Ring(Location _location = OUTER) : location_(_location) {}
        Ring(const BoostRing& _boostRing) : ring_(_boostRing) {}

        void add(point_type _point);

        void fetchLineSegments(std::vector<LineSegment>& _lineSegments) const;
        void fetchVertices(std::vector<Vertex2f>& _vertices) const;
        void resize(scalar_type _distance, std::vector<Ring>& _rings);

        Orientation orientation() const;

        friend void resize(Ring& _in, scalar_type _distance, std::vector<Ring>& _out);
        friend void difference(Ring& _in, std::vector<Ring>& _out);
        friend void unification(Ring& _in, std::vector<Ring>& _out);
        friend bool intersect(Ring& _ring1, Ring& _ring2);

        const BoostRing& operator()() const { return ring_; }
        BoostRing& operator()() { return ring_; }

        TBD_PROPERTY(Location,location);
        TBD_PROPERTY_REF(BoostRing,ring);

      private:
        vec_type getNormal(BoostRing::const_iterator it) const ;
        scalar_type determinant(BoostRing::const_iterator it) const;
        vec_type getNormal(BoostRing::const_iterator _p0, BoostRing::const_iterator _p1) const;
        void nextPrev(BoostRing::const_iterator it,
                      BoostRing::const_iterator& _prev,
                      BoostRing::const_iterator& _next) const;
      };

    }
  }
}

