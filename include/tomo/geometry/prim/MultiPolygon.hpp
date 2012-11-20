#pragma once

#include "Polygon.hpp"
#include <boost/geometry/multi/geometries/register/multi_polygon.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {

      struct MultiPolygon : Primitive2f, std::vector<Polygon>
      {
        typedef std::vector<Polygon> ctnr_type;

        MultiPolygon() :
          correct_(false)
        {

        }
        /// Universal constructor based on assign()
        template<class T> MultiPolygon(const T& _t)
        {
          assign(_t);
        }

        /// universal assign
        template<class INSERT_ITERATOR> 
          void assign(INSERT_ITERATOR _from, INSERT_ITERATOR _to)
        { 
          correct_ = false;
          assign(_from,_to);
        }

        /// fetch line segments out of all polygons
        void fetchSegments(std::vector<Segment>& _segments) const
        {
          for( const Polygon& _polygon : *this )
          {
            _polygon.fetchSegments(_segments);
          }
        }

        void push_back(const Polygon& _polygon)
        {
          correct_ = false;
          ctnr_type::push_back(_polygon);
        }

        void update() 
        {
          if (correct_) return;
          bounds_type _bounds;
          for ( Polygon& _polygon : *this )
          {
            _polygon.update();
            _bounds.extend(_polygon.bounds());
          }
          boost::geometry::correct(*this);
          bounds_=_bounds;
        }

        TBD_PROPERTY_RO(bool,correct)
        TBD_PROPERTY_REF_RO(bounds_type,bounds)
      };
    }
  }
}

BOOST_GEOMETRY_REGISTER_MULTI_POLYGON(tomo::geometry::prim::MultiPolygon)
