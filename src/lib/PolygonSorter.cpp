#include "tomo/geometry/aux/PolygonSorter.hpp"

#include <map>
#include <boost/geometry.hpp>
#include <boost/foreach.hpp>
#include "tomo/geometry/prim/Polygon.hpp"

using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace aux
    {
      using prim::Ring;
      using prim::Polygon;

      PolygonSorter::PolygonSorter(const Rings& _rings, prim::MultiPolygon& _polygons)
      {
        sort(_rings,_polygons);
      }

      void PolygonSorter::sort(const Rings& _rings, prim::MultiPolygon& _polygons) const
      {
        Rings _ringsCopy(_rings);

        /// 1. Step: Union overlapping rings
        /// 2. Step: Find holes

        multimap<Ring*,Ring*> _within;

        BOOST_FOREACH( const Ring& _ring, _rings )
        {
          intersect(_ring,_ring,_ringsCopy);
        }

/*
        for( Rings::iterator it = _ringsCopy.begin(); it != _ringsCopy.end(); ++it )
        {
          map<Rings::iterator,Ring::IntersectResult> _intersectResults;
          

          for( Rings::iterator it1 = it+1; it1 != _ringsCopy.end(); ++it1 )
          {
            
          }
         


        BOOST_FOREACH( Ring& _ring, _rings ) 
        {
          _ring
        }*/
      }
/*
      void PolygonSorter::unification(const Rings& _input, Rings& _output, map<Ring*,Ring*> _within)
      {

      }*/
    }
  }
}
