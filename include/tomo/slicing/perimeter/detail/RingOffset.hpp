#pragma once

#include "tomo/geometry/prim/Ring.hpp"

namespace tomo
{
  namespace slicing
  {
    namespace perimeter
    {
      namespace detail
      {
        template<template<class,class> class RING_CNTR = std::vector>
        struct RingOffset
        {
          typedef geometry::prim::Ring Ring;
          typedef geometry::prim::Polygon Polygon;
          typedef RING_CNTR<Ring,std::allocator<Ring>> Rings;
          typedef Ring::scalar_type scalar_type;
          typedef Ring::point_type point_type;
          typedef Ring::vec_type vec_type;
          

          void operator()(const Ring& _ring, scalar_type _offset, Rings& _outputRings)
          {
            Ring::const_iterator _it0 = _ring.begin(),
                                 _it1 = _it0+1;
            Polygon _output;

            while (1)
            {
              if (_it1 == _ring.end()) break;

              Polygon _polygon;
              point_type _p0 = *_it0, _p1 = *_it1;
              if (_p0 != _p1) 
              {

              generateConnectionPolygon(_p0,_p1,fabs(_offset),_polygon);
              std::vector<Polygon> _unionOutput;
              boost::geometry::union_(_polygon,_output,_unionOutput);

              //TOMO_ASSERT(_unionOutput.size() == 1);
              if (_unionOutput.size() == 1) // continue;
              _output =_unionOutput[0];
              }
              ++_it0;
              ++_it1;
            }

            if (_offset > 0)
            {
              _outputRings.push_back(_output.boundary());
            }
            else
            {
              _outputRings.insert(_outputRings.end(),_output.holes().begin(),_output.holes().end());
            }
          }
          
          /// Overloaded method for extending a single ring
          void operator()(Ring& _ring, scalar_type _offset)
          {
            Rings _outputRings;
            TOMO_ASSERT(_offset >= 0);
            this->operator()(_ring,_offset,_outputRings);
 
            TOMO_ASSERT(_outputRings.size() == 1);

            _ring = _outputRings[0];
          }

        private:
          template<int NUM_SEGMENTS = 48>
          void generateConnectionPolygon( const point_type& _p0,
                                          const point_type& _p1,
                                          scalar_type _offset,
                                          Polygon& _polygon)
          {
            Ring& _ring = _polygon.boundary();

            geometry::prim::Segment _segment(_p0,_p1);
            vec_type _n = -_offset * _segment.normal().normalized();
            _ring.clear();
            _ring.reserve(5+NUM_SEGMENTS/2);
            _ring.push_back(_p0 +(-_n));

            for (int i = 0; i < NUM_SEGMENTS; i++)
            {
              scalar_type _angle = i*M_PI*2.0/NUM_SEGMENTS;
              scalar_type _sin = _offset*sin(_angle),
                          _cos = _offset*cos(_angle);
              if (_n.y() > 0)
              {
                _sin = -_sin;
                _cos = -_cos;
              }
              vec_type _v(-_sin,_cos);

              if (dot(_v,_n) > 0)
              {
                point_type _p = point_type(_cos,_sin) + _p0;
                _ring.push_back(_p);
              }
            }

            _ring.push_back(_p0 + _n);
            _ring.push_back(_p1 + _n);
            _ring.push_back(_p1 +(-_n));
            boost::geometry::correct(_ring);
          }
        };
      }
    }
  }
}
