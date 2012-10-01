#include "tomo/geometry/prim/Polygon.hpp"

#include <boost/foreach.hpp>
#include <tbd/log.h>
#include <math.h>

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
          set<float> _segMarkers;
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
        float _sin = _radius * sin(_angle*M_PI/100);

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

      Polygon Polygon::shrinked(float _factor) const
      {
        Polygon _polygon;
        
        _polygon().outer() = shrinked(_factor,polygon_.outer());

        BOOST_FOREACH( const Ring& _inner, polygon_.inners() )
        {
          _polygon().inners().push_back(shrinked(-_factor,_inner));
        }

        return _polygon;
      }

      Ring Polygon::shrinked(float _factor, const Ring& _in) const
      {
        Ring _ring;
        _ring.reserve(polygon_.outer().size());
/*        
        PointXYf _lastPoint = _ring[_ring.size()-1].
        BOOST_FOREACH ( PointXYf _point, _ring );
        {

          Point2f _shrinkPoint(
          _polygon.addOuter(_shrinkPoint);

          _lastPoint = _point;
        }
*/
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
        



      /*
        void Polygon::calcType()
        {
        }

        void Polygon::calcBounds()
        {
        }

        void sortPolygons(Polygon& _polygon, std::list<Polygon>& _polygons)
        {


          _polygon.children().push_back(*it);

          sortPolygons(_polygons
        }


        void sortPolygons(std::list<Polygon>& _polygons)
        {
         std::list<Polygon>::iterator it = _polygons.begin();

         while


         for (
        }
        */
    }
  }
}
