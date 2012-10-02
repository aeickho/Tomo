#pragma once

#include <vector>
#include <list>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include "tomo/geometry/prim/Primitive.hpp"
#include "tomo/geometry/prim/LineSegment.hpp"
#include "tomo/geometry/aux/Bounds.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      ///@todo: merge PointXYf with Point2f
      typedef boost::geometry::model::d2::point_xy<float> PointXYf;
      typedef boost::geometry::model::polygon<PointXYf> BoostPolygon;
      typedef boost::geometry::model::ring<PointXYf> Ring;

      struct Polygon : public Primitive2f
      {
        typedef enum { PT_NONE, PT_CLOSURE, PT_HOLE } Type;

        bounds_type bounds() const
        {
          return bounds_;
        }

        bool intersect(ray_type& _ray, float& _tNear, float& _tFar, vector_type* _normal = NULL) const
        {
          return false;
        }

        void lineSegments(ray_type& _ray, std::vector<LineSegment>& _lineSegments ) const;
        std::vector<LineSegment> fetchLineSegments() const;

        void addOuter(point_type _point);
        void boundingRays(float _angle, ray_type& _rayBegin, ray_type& _rayEnd) const;

        Polygon shrinked(float _factor) const;

        const BoostPolygon& operator()() const { return polygon_; }
        BoostPolygon& operator()() { return polygon_; }

        TBD_PROPERTY(BoostPolygon,polygon);
      
      private:
        bounds_type bounds_;

        void lineSegmentsFromRing(
            const Ring& _ring, 
            std::vector<LineSegment>& _lineSegments) const;

        Ring shrinked(float _factor, const Ring& _in) const;

        void lineSegmentsFromSegMarkers(
            const ray_type& _ray,
            const std::set<float>& _segMarkers,
            std::vector<LineSegment>& _lineSegments) const;
        
      };

      typedef std::vector<Polygon> MultiPolygon;
    }
  }
}

