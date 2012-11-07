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
        enum Location { INNER, OUTER };

        /// Orientation of ring (CW = clockwise, CCW = counter-clockwise)
        enum Orientation { CW, CCW };

        /// Default constructor
        Ring(Location _location = OUTER);

        void add(point_type _point);

        void fetchSegments(std::vector<Segment>& _segments) const;
        void fetchSegments(
          const Segment& _segment,
          std::vector<Segment>& _segments) const;
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

        TBD_PROPERTY(Location,location);
      private:
        vec_type getNormal(Ring::const_iterator _p0, Ring::const_iterator _p1) const;
      };
    }
  }
}

BOOST_GEOMETRY_REGISTER_RING(tomo::geometry::prim::Ring)

namespace boost
{
  namespace serialization
  { 
    template<class ARCHIVE, class TYPE>
    void load(
        ARCHIVE& _ar, 
        boost::geometry::model::d2::point_xy<TYPE>& _p, 
        const unsigned int _fileVersion)
    {
      TYPE x,y;
      _ar >> x >> y;
      _p.set<0>(x);
      _p.set<1>(y);
    }
    template<class ARCHIVE, class TYPE>
    void save(
        ARCHIVE& _ar, 
        const boost::geometry::model::d2::point_xy<TYPE>& _p, 
        const unsigned int _fileVersion)
    {
      _ar << _p.template get<0>() << _p.template get<1>();
    }
    template<class ARCHIVE, class TYPE>
    void serialize(
        ARCHIVE& _ar, 
        boost::geometry::model::d2::point_xy<TYPE>& _p, 
        const unsigned int _fileVersion)
    {
      split_free(_ar,_p,_fileVersion);
    }

    template<class ARCHIVE>
    void serialize(
        ARCHIVE& _ar, 
        tomo::geometry::prim::Ring& _boostRing, 
        const unsigned int _fileVersion)
    {
      _ar & (std::vector<tomo::geometry::base::Point2f> &)_boostRing;
    }
  }
}
