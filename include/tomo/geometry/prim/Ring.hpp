#pragma once

#include <boost/geometry/geometries/ring.hpp>
#include <boost/serialization/vector.hpp>

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
        enum IntersectResult { DISJOINT, OVERLAP, A_WITHIN_B, B_WITHIN_A };
        enum Location { INNER, OUTER };

        /// Orientation of ring (CW = clockwise, CCW = counter-clockwise)
        enum Orientation { CW, CCW };

        /// Default constructor
        Ring(Location _location = OUTER);

        /// Boost wrapper constructor
        Ring(const BoostRing& _boostRing);

        void add(point_type _point);

        void fetchLineSegments(std::vector<LineSegment>& _lineSegments) const;
        void fetchLineSegments(
          const LineSegment& _lineSegment,
          std::vector<LineSegment>& _lineSegments) const;
        void fetchLineSegments(BoostRing::const_iterator it1,
                               BoostRing::const_iterator it2,
                               std::vector<LineSegment>& _lineSegments) const;

        void fetchVertices(std::vector<Vertex2f>& _vertices) const;
        void resize(scalar_type _distance, std::vector<Ring>& _rings);

        friend IntersectResult intersect(const Ring& _a, const Ring& _b, std::vector<Ring>& _out);

        Orientation orientation() const;

        const BoostRing& operator()() const
        {
          return ring_;
        }
        BoostRing& operator()()
        {
          return ring_;
        }

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

namespace boost
{
  namespace serialization
  {  
    template<class ARCHIVE>
    void serialize(
        ARCHIVE& _ar, 
        tomo::geometry::base::BoostPoint2& _boostPoint2, 
        const unsigned int _fileVersion)
    {
      _ar & tomo::geometry::base::Point2f(_boostPoint2);
    }


    template<class ARCHIVE>
    void serialize(
        ARCHIVE& _ar, 
        tomo::geometry::prim::BoostRing& _boostRing, 
        const unsigned int _fileVersion)
    {
      _ar & _boostRing;
    }
  }
}
