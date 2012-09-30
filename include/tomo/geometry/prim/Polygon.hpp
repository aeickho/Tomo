#pragma once

#include <vector>
#include <list>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include "tomo/geometry/prim/Primitive.hpp"
#include "tomo/geometry/aux/Bounds.hpp"

namespace tomo
{
  namespace geometry
  {
    namespace prim
    {
      typedef boost::geometry::model::d2::point_xy<float> PointXYf;
      typedef boost::geometry::model::polygon<PointXYf> Polygon;
      typedef boost::geometry::model::ring<PointXYf> Ring;

      /*
      struct Polygon : public Primitive2f
      {
      typedef enum { PT_NONE, PT_CLOSURE, PT_HOLE } Type;

      Bounds2f bounds() const { return bounds_; }
      bool intersect(Ray2f& _ray, float& _tNear, float& _tFar, Vec2f* _normal = NULL) const
      {
      return false;
      }

      TODO:
      struct Ring : Compound<Point2f,2,float>
      {
      typedef enum { WITHIN, INTERSECT, DISJOINT } RingIntersectResult;
      void simplify();
      friend RingIntersectResult intersect(Ring& a, Ring& b);

      intersect(ray& _ray);


      Type { Closure, Hole }


      TBD_PROPERTY_REF(std::vector<Point2f>,points);
      TBD_PROPERTY_REF(std::list<Ring>,holes);
      TBD_PROPERTY_REF(std::list<Ring>,children);

      private:
      RingIntersectResult intersect(LineSegment);
      };



      TBD_PROPERTY_REF(std::list<Polygon>,children);
      TBD_PROPERTY_RO(Type,type);

      private:
      void calcBounds();
      void calcType();

      Bounds2f bounds_;
      };
      */
      typedef std::vector<Polygon> MultiPolygon;
    }
  }
}

