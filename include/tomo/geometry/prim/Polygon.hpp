#pragma once

#include <boost/geometry.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include "Ring.hpp"
#include "tomo/geometry/base/Bounds.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      struct Polygon : Primitive2f
      {
        typedef std::vector<Ring> holes_type;

        Polygon() {}

        Polygon(const Ring& _boundary) :
          correct_(false),
          boundary_(_boundary)
        {}

        Polygon(const Ring& _boundary, const std::vector<Ring>& _holes) :
          correct_(false),
          boundary_(_boundary),
          holes_(_holes)
        {
        }

        void fetchSegments(std::vector<Segment>& _segments) const
        {
          boundary_.fetchSegments(_segments);

          for( const Ring& _ring : holes_ )
          {
            _ring.fetchSegments(_segments);
          }
        }

        void add(const Ring& _ring)
        {
          switch (_ring.location())
          {
          case Ring::INNER:
            holes_.push_back(_ring);
            break;
          case Ring::OUTER:
            boundary_ = _ring;
            break;
          }
          correct_ = false;
        }

        template<class ARCHIVE>
        void serialize( ARCHIVE& _ar, const unsigned int _fileVersion )
        {
          _ar & boundary_;
          _ar & holes_;
        }

        void update()
        {
          if (correct_) return;
          bounds_type _bounds;
          boundary_.update();
          _bounds.extend(boundary_.bounds());

          for( Ring& _ring : holes_ )
          {
            _ring.update();
            _bounds.extend(_ring.bounds());
          }

          boost::geometry::correct(*this);
          bounds_=_bounds;
        }

        holes_type& holes()
        {
        	correct_ = false;
        	return holes_;
        }
 
        TBD_PROPERTY_RO(bool,correct)
        TBD_PROPERTY_REF(Ring,boundary)
        TBD_PROPERTY_REF_RO(bounds_type,bounds)
        TBD_PROPERTY_REF_RO(holes_type,holes);
      };
    }
  }
}

/// Boost Geometry Polygon Type Registration
namespace boost
{
  namespace geometry
  {
    namespace traits
    {
      using tomo::geometry::prim::Ring;
      using tomo::geometry::prim::Polygon;

      template<> struct tag<Polygon>
      {
        typedef polygon_tag type;
      };
      template<> struct ring_const_type<Polygon>
      {
        typedef Ring const& type;
      };
      template<> struct ring_mutable_type<Polygon>
      {
        typedef Ring& type;
      };

      template<> struct interior_const_type<Polygon>
      {
        typedef std::vector<Ring> const& type;
      };

      template<> struct interior_mutable_type<Polygon>
      {
        typedef std::vector<Ring>& type;
      };

      template<> struct exterior_ring<Polygon>
      {
        static Ring& get(Polygon& p)
        {
          return p.boundary();
        }

        static Ring const& get(Polygon const& p)
        {
          return p.boundary();
        }
      };

      template<> struct interior_rings<Polygon>
      {
        typedef std::vector<Ring> holes_type;

        static holes_type& get(Polygon& p)
        {
          return p.holes();
        }

        static holes_type const& get(Polygon const& p)
        {
          return p.holes();
        }
      };
    }
  }
} // namespace boost::geometry::traits

