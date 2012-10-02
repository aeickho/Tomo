#include "tomo/geometry/prim/Polygon.hpp"

#include <boost/foreach.hpp>
#include <tbd/log.h>
#include <math.h>
#include <boost/geometry/geometries/adapted/c_array.hpp>
#include <boost/geometry/algorithms/buffer.hpp>
#include <boost/geometry/multi/geometries/register/multi_polygon.hpp>

using namespace boost::geometry;

BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian);

BOOST_GEOMETRY_REGISTER_MULTI_POLYGON(tomo::geometry::prim::BoostMultiPolygon);
//BOOST_GEOMETRY_REGISTER_MULTI_POLYGON(tomo::geometry::prim::MultiPolygon);

using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      void Polygon::addOuter(point_type _point)
      {
        polygon_.outer().push_back(PointXYf(_point.x(),_point.y()));
        bounds_.extend(_point);
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
        float _radius = bounds_.radius();
        float _cos = _radius * cos(_angle*M_PI/180); 
        float _sin = _radius * sin(_angle*M_PI/180);

        vector_type _cross(-_sin,_cos);
        _rayBegin.dir(vector_type(2*_cos,2*_sin));
        _rayEnd.dir(_rayBegin.dir());
        _rayBegin.tNear(0.0);
        _rayBegin.tFar(1.0);
        _rayEnd.tNear(0.0);
        _rayEnd.tFar(1.0);

        point_type _center = bounds_.center();
        _rayBegin.org(_center - _cross - vector_type(_cos,_sin));
        _rayEnd.org(_center + _cross - vector_type(_cos,_sin));
      }

      vector<LineSegment> Polygon::fetchLineSegments() const
      {
        vector<LineSegment> _lineSegments;
        
        // Fetch outer segments
        const Ring& _outer = polygon_.outer();
        lineSegmentsFromRing(_outer,_lineSegments);

        // Fetch inner segments
        BOOST_FOREACH( const Ring& _inner, polygon_.inners() )
          lineSegmentsFromRing(_inner,_lineSegments);

        return _lineSegments;
      }

      BoostMultiPolygon Polygon::shrinked(float _factor) const
      {
/*        _polygon().outer() = shrinked(_factor,polygon_.outer());

        BOOST_FOREACH( const Ring& _inner, polygon_.inners() )
        {
          _polygon().inners().push_back(shrinked(-_factor,_inner));
        }
*/
        return BoostMultiPolygon();
    //    return boost::geometry::return_buffer<BoostMultiPolygon,BoostPolygon,float>(polygon_,_factor);
      }

      Ring Polygon::shrinked(float _factor, const Ring& _in) const
      {
        if (_in.size() < 3) return Ring();
        Ring _ring;
        


        size_t _n = _in.size();
        
        _ring.reserve(_n);

        using boost::geometry::get;
        point_type _lastPoint(get<0>(_in[_n-1]),
                           get<1>(_in[_n-1]));
        for (size_t i = 0; i < _in.size()-1; i++)
        {
          point_type _nextPoint(get<0>(_in[i+1]),get<1>(_in[i+1]));
          point_type _point(get<0>(_in[i]),get<1>(_in[i]));
          vector_type _normal = ( _point - _lastPoint.vec() + _point - _nextPoint.vec()).normalized();
          point_type _newPoint = _point + _factor * _normal;
          _ring.push_back(PointXYf(_newPoint.x(),_newPoint.y()));
          point_type _lastPoint = _point;
        }

        point_type _nextPoint(get<0>(_in[0]),get<1>(_in[0]));
        point_type _point(get<0>(_in[_n-1]),get<1>(_in[_n-1]));
          vector_type _normal = ( _point - _lastPoint.vec() + _point - _nextPoint.vec()).normalized();        
        point_type _newPoint = _point + _factor * _normal;
        _ring.push_back(PointXYf(_newPoint.x(),_newPoint.y()));

        LOG_MSG << _ring.size();

        return _ring;
      }

      void Polygon::lineSegmentsFromRing(
          const Ring& _ring,
          vector<LineSegment>& _lineSegments) const
      {
        _lineSegments.reserve( _lineSegments.size() + _ring.size() ); 
        using boost::geometry::get;
        
        point_type _points[2];
        int i = 0;
        if (_ring.size() < 2) return;

        BOOST_FOREACH( const PointXYf& _point, _ring )
        {
          _points[i % 2](get<0>(_point),get<1>(_point));
          if (i > 0) 
          {
            _lineSegments.push_back(LineSegment(_points[i % 2],_points[1 - i % 2]));
          }
          i++;  
        }
      
        _lineSegments.push_back(LineSegment(_points[1-_ring.size() % 2],
              point_type(get<0>(_ring[0]),get<1>(_ring[0]))));
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
