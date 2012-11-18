#pragma once

#include "RingOffset.hpp"
#include "Unify.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace algorithm
    {
      template<template<class,class> class POLYGON_CNTR = std::vector>
      struct PolygonOffset
      {
        typedef geometry::prim::Ring Ring;
        typedef geometry::prim::Polygon Polygon;
        typedef POLYGON_CNTR<Polygon,std::allocator<Polygon>> Polygons;
        typedef Polygon::scalar_type scalar_type;
        typedef Polygon::point_type point_type;
        typedef Polygon::vec_type vec_type;

        void operator()(
          const Polygon& _input,
          scalar_type _offset,
          POLYGON_CNTR<Polygon,std::allocator<Polygon>>& _output)
        {
          Ring _outer(_input.boundary());

          std::vector<Ring> _outerRings;
          RingOffset<>()(_outer,_offset,_outerRings);

          std::vector<Ring> _innerRings;
          for ( const Ring& _inner : _input.holes() )
            RingOffset<>()(_inner,-_offset,_innerRings);

          std::vector<Ring> _innerRingsUnified;
          Unify()(_innerRings,_innerRingsUnified);

          std::vector<Polygon> _innerPolygons;
          _innerPolygons.reserve(_innerRingsUnified.size());
          for ( const Ring& _inner : _innerRingsUnified )
          {
            Polygon _polygon(_inner);
            boost::geometry::correct(_polygon.boundary());
            _innerPolygons.push_back(_polygon);
          }

          std::vector<Polygon> _outerPolygons;
          _outerPolygons.reserve(_outerRings.size());
          for ( const Ring& _outer : _outerRings )
          {
            Polygon _polygon(_outer);
            boost::geometry::correct(_polygon.boundary());
            _outerPolygons.push_back(_polygon);
          }

          boost::geometry::difference(_outerPolygons,_innerPolygons,_output);
        }
      };
    }
  }
}
