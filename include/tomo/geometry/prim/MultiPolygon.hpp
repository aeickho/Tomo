#pragma once

#include "Polygon.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      struct MultiPolygon : Primitive2f
      {
        MultiPolygon()
        {
        }
        /// Universal constructor based on assign()
        template<class T> MultiPolygon(const T& _t)
        {
          assign(_t);
        }

        /// assign from vector<Ring>
        void assign(const std::vector<Ring>& _rings)
        {
          TOMO_NOT_IMPLEMENTED();
          polygons_.clear();
          std::vector<Ring> _outerRings;
          //determineOuterRings(_rings,_outerRings);
          //determineHoles(_rings,_outerRings);
          //BOOST_FOREACH( Ring& _ring, _outerRings )
          {
          }
        }
        /// universal assign
        template<class INSERT_ITERATOR> 
          void assign(INSERT_ITERATOR _from, INSERT_ITERATOR _to)
        { 
          polygons_.assign(_from,_to);
        }
        /// fetch line segments out of all polygons
        void fetchSegments(std::vector<Segment>& _segments) const
        {
          for( const Polygon& _polygon : polygons_ )
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

        TBD_PROPERTY_REF(std::vector<Polygon>,polygons);
      };
    }
  }
}
