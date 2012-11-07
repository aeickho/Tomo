#pragma once

#include "RingOffset.hpp"

namespace tomo
{
  namespace slicing
  {
    namespace perimeter
    {
      namespace detail
      {
        template<template<class,class> class POLYGON_CNTR = std::vector>
        struct PolygonOffset
        {
          typedef geometry::prim::Ring Ring;
          typedef geometry::prim::BoostRing BoostRing;
          typedef geometry::prim::Polygon Polygon;
          typedef geometry::prim::BoostPolygon BoostPolygon;
          typedef POLYGON_CNTR<Polygon,std::allocator<Polygon>> Polygons;
          typedef Polygon::scalar_type scalar_type;
          typedef Polygon::point_type point_type;
          typedef Polygon::vec_type vec_type;

          void operator()(
            const Polygon& _input,
            scalar_type _offset,
            POLYGON_CNTR<Polygon,std::allocator<Polygon>>& _output)
          {
            const BoostPolygon& _bP = _input;
            Ring _outer(_bP.outer());
            
            std::vector<Ring> _outerRings;
            RingOffset<>()(_outer,_offset,_outerRings);

            std::vector<Ring> _innerRings;
            for ( const BoostRing& _inner : _bP.inners() )
              RingOffset<>()(_inner,-_offset,_innerRings);

            std::vector<BoostPolygon> _innerPolygons;
            _innerPolygons.reserve(_innerRings.size());
            for ( const Ring& _inner : _innerRings )
            {
              BoostPolygon _bP;
              _bP.outer() = _inner();
              boost::geometry::correct(_bP.outer());
              _innerPolygons.push_back(_bP);
            }

            std::vector<BoostPolygon> _outerPolygons;
            _outerPolygons.reserve(_outerRings.size());
            for ( const Ring& _outer : _outerRings )
            {
              BoostPolygon _bP;
              _bP.outer() = _outer();
              boost::geometry::correct(_bP.outer());
              _outerPolygons.push_back(_bP);
            }

            std::vector<BoostPolygon> _outputPolygons;
            boost::geometry::difference(_outerPolygons,_innerPolygons,_outputPolygons);

            for ( const BoostPolygon& _p : _outputPolygons )
              _output.push_back(Polygon(_p));
          }
        };
      }
    }
  }
}
