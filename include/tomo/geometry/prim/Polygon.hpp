#pragma once

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/foreach.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include "Ring.hpp"
#include "tomo/geometry/base/Bounds.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      typedef boost::geometry::model::polygon<base::BoostPoint2> BoostPolygon;

      struct Polygon : Primitive2f
      {
        Polygon() {}
        Polygon(const BoostPolygon& _boostPolygon) : 
          polygon_(_boostPolygon)
        {}
      
        Polygon(const Ring& _outer, const std::vector<Ring>& _inners)
        {
          polygon_.outer() = _outer();
          BOOST_FOREACH ( const Ring& _inner, _inners )
          {
            polygon_.inners().push_back(_inner());
          }
        }

        void fetchSegments(std::vector<Segment>& _segments) const
        {
          Ring _outer(polygon_.outer());
          _outer.fetchSegments(_segments);

          BOOST_FOREACH( const BoostRing& _ring, polygon_.inners() )
          {
            Ring _inner(_ring);
            _inner.fetchSegments(_segments);
          }
        }

        void add(const Ring& _ring)
        {
          switch (_ring.location())
          {
          case Ring::INNER:
            polygon_.inners().push_back(_ring());
            break;
          case Ring::OUTER:
            polygon_.outer() = _ring();
            break;
          }
        }

        operator const BoostPolygon& () const
        {
          return polygon_;
        }
        operator BoostPolygon& ()
        {
          return polygon_;
        }

        template<class ARCHIVE>
        void serialize( ARCHIVE& _ar, const unsigned int _fileVersion )
        {
          _ar & polygon_;
          _ar & bounds_;
        }

        TBD_PROPERTY(BoostPolygon,polygon);
        TBD_PROPERTY_REF(bounds_type,bounds);
      };
    }
  }
}

namespace boost
{
  namespace serialization
  {
    using tomo::geometry::prim::BoostPolygon;

    template<class ARCHIVE>
    void serialize(ARCHIVE& _ar, BoostPolygon& _boostPolygon, const unsigned int _fileVersion)
    {
      _ar & _boostPolygon.outer();
      _ar & _boostPolygon.inners();
    }
  }
}
