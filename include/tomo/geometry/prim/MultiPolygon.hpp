#pragma once

#include "Polygon.hpp"
#include <boost/geometry/multi/geometries/register/multi_polygon.hpp>

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
/*
      struct MultiPolygon : Primitive2f, std::vector<Polygon>
      {
        MultiPolygon()
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

        bounds_type bounds() const 
        {
          TOMO_NOT_IMPLEMENTED();
          ///@todo MultiPolygon should hold a KDTree as member (which already has bounds)
          return bounds_type(); 
        }
      };
*/
      typedef std::vector<Polygon> MultiPolygon; 
    }
  }
}

BOOST_GEOMETRY_REGISTER_MULTI_POLYGON(tomo::geometry::prim::MultiPolygon)
