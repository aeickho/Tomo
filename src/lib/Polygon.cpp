#include "tomo/geometry/prim/Polygon.hpp"

#include <boost/foreach.hpp>

using namespace std;

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      void Polygon::lineSegments(Ray2f& _ray, std::vector<LineSegment>& _lineSegments ) const
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

      std::vector<LineSegment> Polygon::lineSegmentsFromRing(
          const Ring& _ring,
          vector<LineSegment>& _lineSegments) const
      {
        _lineSegments.reserve( _lineSegments.size() + _ring.size() ); 
        base::Point2f _points[2];
        int i = 0;
        BOOST_FOREACH( const PointXYf& _point, _ring )
        {
          _points[i % 2](boost::geometry::get<0>(_point),
                         boost::geometry::get<1>(_point));

          _lineSegments.push_back(LineSegment(_points[0],_points[1]));
          swap(_points[0],_points[1]);
          i++;  
        }
        return _lineSegments;
      }
        
      void Polygon::lineSegmentsFromSegMarkers(
          const Ray2f& _ray,
          const std::set<float>& _segMarkers,
          std::vector<LineSegment>& _lineSegments) const
      {
        int i = 0;
        _lineSegments.reserve(_lineSegments.size() + _segMarkers.size()/2);
        base::Point2f _points[2];
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
