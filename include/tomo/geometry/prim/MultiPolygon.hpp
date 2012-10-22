#pragma once

#include "Polygon.hpp"
#include <boost/foreach.hpp>

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
        void fetchLineSegments(std::vector<LineSegment>& _lineSegments) const
        {
          BOOST_FOREACH( const Polygon& _polygon, polygons_ )
          {
            _polygon.fetchLineSegments(_lineSegments);
          }
        }

        TBD_PROPERTY_REF(std::vector<Polygon>,polygons);
      };
    }
  }
}
