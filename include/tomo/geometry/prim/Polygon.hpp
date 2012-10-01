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
      typedef boost::geometry::model::d2::point_xy<float> PointXYf;
      typedef boost::geometry::model::polygon<PointXYf> BoostPolygon;
      typedef boost::geometry::model::ring<PointXYf> Ring;

      struct Polygon : public Primitive2f
      {
        typedef enum { PT_NONE, PT_CLOSURE, PT_HOLE } Type;
        typedef aux::Bounds2f Bounds2f;

        Bounds2f bounds() const
        {
          return bounds_;
        }

        bool intersect(aux::Ray2f& _ray, float& _tNear, float& _tFar, base::Vec2f* _normal = NULL) const
        {
          return false;
        }

        void lineSegments(const aux::Ray2f& _ray, std::vector<LineSegment>& _lineSegments ) const
        {
        }

        const BoostPolygon& operator()() const { return polygon_; }
        BoostPolygon& operator()() { return polygon_; }

        TBD_PROPERTY(BoostPolygon,polygon);
      
      private:
        Bounds2f bounds_;

        std::vector<LineSegment> fetchLineSegments();
        
      };

      typedef std::vector<Polygon> MultiPolygon;
    }
  }
}

