#include "tomo/geometry/prim/Polygon.hpp"

#include <boost/foreach.hpp>
#include <tbd/log.h>
#include <math.h>
#include <boost/assert.hpp>

using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      void Polygon::fetch(Ring::Location& _location, ptr_vector_type& _output)
      {
        BOOST_FOREACH( Ring& _ring, rings() )
        {
          if (_ring.location() == _location)
          {
            _output.push_back(&_ring);
          }
        }
      }

      void Polygon::lineSegments(ray_type& _ray, std::vector<LineSegment>& _lineSegments ) const
      {
        std::set<float> _segMarkers;
        vector<LineSegment> _polygonSegments = fetchLineSegments();

        BOOST_FOREACH( const LineSegment& _lineSegment, _polygonSegments )
        {
          float _tNear = 0.0, _tFar = 1.0;
          if (_lineSegment.intersect(_ray,_tNear,_tFar))
          {
            _segMarkers.insert(_ray.tFar());
            _ray.tFar(1.0);
          }
        }

        lineSegmentsFromSegMarkers(_ray,_segMarkers,_lineSegments);
      }

      void Polygon::boundingRays(float _angle, ray_type& _rayBegin, ray_type& _rayEnd) const
      {
        float _radius = bounds().radius();
        float _cos = _radius * cos(_angle*M_PI/180);
        float _sin = _radius * sin(_angle*M_PI/180);

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

      vector<LineSegment> Polygon::fetchLineSegments() const
      {
        vector<LineSegment> _lineSegments;
        /*
                // Fetch outer segments
                const Ring& _outer = polygon_.outer();
                lineSegmentsFromRing(_outer,_lineSegments);

                // Fetch inner segments
                BOOST_FOREACH( const Ring& _inner, polygon_.inners() )
                lineSegmentsFromRing(_inner,_lineSegments);
        */
        return _lineSegments;
      }


      void Polygon::lineSegmentsFromSegMarkers(
        const ray_type& _ray,
        const std::set<float>& _segMarkers,
        std::vector<LineSegment>& _lineSegments) const
      {
        int i = 0;
        _lineSegments.reserve(_lineSegments.size() + _segMarkers.size()/2);
        point_type _points[2];
        BOOST_FOREACH( const float& _segMarker, _segMarkers )
        {
          _points[i % 2] = _ray.org() + _segMarker * _ray.dir();
          if (i % 2 == 1)
          {
            _lineSegments.push_back(LineSegment(_points[0],_points[1]));
          }
          i++;
        }
      }
    }
  }
}
