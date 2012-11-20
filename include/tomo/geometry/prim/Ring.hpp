#pragma once

#include "Segment.hpp"
#include "Vertex.hpp"

#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/nvp.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      struct Ring  : Primitive2f, std::vector<base::Point2f> 
      {
        enum IntersectResult { DISJOINT, OVERLAP, A_WITHIN_B, B_WITHIN_A };
        enum Location : unsigned char { INNER, OUTER };
        typedef std::vector<base::Point2f> ctnr_type;

        /// Orientation of ring (CW = clockwise, CCW = counter-clockwise)
        enum Orientation { CW, CCW };

        /// Default constructor
        Ring(Location _location = OUTER);

        void clear();
        void push_back(point_type _point);

        void update();

        void fetchSegments(std::vector<Segment>& _segments) const;
        void fetchSegments(Ring::const_iterator it1,
                           Ring::const_iterator it2,
                           std::vector<Segment>& _segments) const;

        void fetchVertices(std::vector<Vertex2f>& _vertices) const;

        friend IntersectResult intersect(const Ring& _a, const Ring& _b, std::vector<Ring>& _out);

        scalar_type determinant(Ring::const_iterator it) const;
        void prevNext(Ring::const_iterator it,
                      Ring::const_iterator& _prev,
                      Ring::const_iterator& _next) const;
        
        Orientation orientation() const;
        vec_type getNormal(Ring::const_iterator it) const ;
        
        const bounds_type& bounds() const
        {
          return bounds_;
        }

        TBD_PROPERTY(Location,location)
        TBD_PROPERTY_RO(bool,correct)
        TBD_PROPERTY_RO(scalar_type,length)

      private:
        static vec_type getNormal(Ring::const_iterator _p0, Ring::const_iterator _p1);

        bounds_type bounds_;
      };
    }
  }
}

BOOST_GEOMETRY_REGISTER_RING(tomo::geometry::prim::Ring)

