#include "tomo/geometry/prim/Polygon.hpp"

#include <boost/geometry/geometries/adapted/c_array.hpp>
#include <boost/foreach.hpp>
#include <tbd/log.h>
#include <math.h>
#include <boost/assert.hpp>

using namespace std;

using namespace boost::geometry;

BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian);

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      Polygon::Polygon()
      {
      }

      Polygon::Polygon(const BoostPolygon& _boostPolygon) : 
        polygon_(_boostPolygon)
      {
      }

      Polygon::Polygon(const Ring& _outer, const std::vector<Ring>& _inners)
      {
        polygon_.outer() = _outer();
        BOOST_FOREACH ( const Ring& _inner, _inners )
        {
          polygon_.inners().push_back(_inner());
        }
      }

      void Polygon::segments(ray_type& _ray, std::vector<Segment>& _segments ) const
      {
        /// @todo Use kdtree for polygon ray intersection 
        std::set<scalar_type> _segMarkers;
        vector<Segment> _polygonSegments; 
        fetchSegments(_polygonSegments);
/*
        BOOST_FOREACH( const Segment& _segment, _polygonSegments )
        {
          scalar_type _tNear = 0.0, _tFar = 1.0;
          if (_segment.intersect(_ray,_tNear,_tFar))
          {
            _segMarkers.insert(_ray.tFar());
            _ray.tFar(1.0);
          }
        }*/

        segmentsFromSegMarkers(_ray,_segMarkers,_segments);
      }

      void Polygon::boundingRays(scalar_type _angle, ray_type& _rayBegin, ray_type& _rayEnd) const
      {
        scalar_type _radius = bounds().radius();
        scalar_type _cos = _radius * cos(_angle*M_PI/180);
        scalar_type _sin = _radius * sin(_angle*M_PI/180);

        vec_type _cross(-_sin,_cos);
        _rayBegin.dir(vec_type(2*_cos,2*_sin));
        _rayEnd.dir(_rayBegin.dir());
        _rayBegin.tNear(0.0);
        _rayBegin.tFar(1.0);
        _rayEnd.tNear(0.0);
        _rayEnd.tFar(1.0);

        point_type _center = bounds().center();
        _rayBegin.org(_center - _cross - vec_type(_cos,_sin));
        _rayEnd.org(_center + _cross - vec_type(_cos,_sin));
      }

      void Polygon::fetchSegments(vector<Segment>& _segments) const
      {
        Ring _outer(polygon_.outer());
        _outer.fetchSegments(_segments);
       
        BOOST_FOREACH( const BoostRing& _ring, polygon_.inners() )
        {
          Ring _inner(_ring);
          _inner.fetchSegments(_segments);
        }
      }

      void Polygon::segmentsFromSegMarkers(
        const ray_type& _ray,
        const std::set<float>& _segMarkers,
        std::vector<Segment>& _segments) const
      {
        int i = 0;
        _segments.reserve(_segments.size() + _segMarkers.size()/2);
        point_type _points[2];
        BOOST_FOREACH( const float& _segMarker, _segMarkers )
        {
          _points[i % 2] = _ray.org() + _segMarker * _ray.dir();
          if (i % 2 == 1)
          {
            _segments.push_back(Segment(_points[0],_points[1]));
          }
          i++;
        }
      }

      void Polygon::add(const Ring& _ring)
      {
        switch (_ring.location())
        {
          case Ring::INNER: polygon_.inners().push_back(_ring()); break;
          case Ring::OUTER: polygon_.outer() = _ring(); break;
        }
      }
    }
  }
}
